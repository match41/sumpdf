/***************************************************************************\
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2.                              *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
\***************************************************************************/

/**	\file	BaseEncoding.hh
    \brief	definition the BaseEncoding class
    \date	Apr 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_BASEENCODING_HH_EADER_INCLUDED__
#define __PDF_BASEENCODING_HH_EADER_INCLUDED__

// base class headers
#include "util/RefCounter.hh"
#include "font/FontEncoding.hh"

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

namespace pdf {

class Object ;
class File ;

///	brief description
/**	\internal
	The BaseEncoding class represents
*/
class BaseEncoding : public RefCounter, public FontEncoding
{
public :
	typedef Object BaseType ;

public :
	~BaseEncoding( ) ;

	virtual Object Write( File *file ) const ;

	virtual wchar_t ToUnicode( unsigned short char_code ) const ;
	virtual unsigned short FromUnicode( wchar_t unicode ) const ;

	std::wstring Decode( const std::string& bytes ) const ;
	std::size_t Encode(
		std::wstring::const_iterator first,
		std::wstring::const_iterator last,
		std::ostream& out ) const ;

	void Add( unsigned short char_code, wchar_t unicode ) ;

protected :
	typedef	boost::bimap<
		boost::bimaps::unordered_set_of<unsigned short>,
		boost::bimaps::unordered_set_of<wchar_t>
	> CharMap ; 

	typedef CharMap::left_iterator			iterator ;
	typedef CharMap::left_const_iterator	const_iterator ;

	iterator begin( ) ;
	iterator end( ) ;
	const_iterator begin( ) const ;
	const_iterator end( ) const ;

private :
	static unsigned short Ext( char ch ) ;

private :
	/// mapping from character code to unicode
	CharMap	m_charmap ;
} ;

BaseEncoding* CreateEncoding( Object& obj, File *file ) ;

} // end of namespace

#endif // BASEENCODING_HH_
