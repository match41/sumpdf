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

/**	\file	FontEncoding.hh
    \brief	definition the FontEncoding class
    \date	Mar 21, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTENCODING_HH_EADER_INCLUDED__
#define __PDF_FONTENCODING_HH_EADER_INCLUDED__

#include "util/RefCounter.hh"

#include <string>
#include <map>

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

#include <iosfwd>

namespace pdf {

class DictReader ;

///	brief description
/**	\internal
	The FontEncoding class represents
*/
class FontEncoding : public RefCounter
{
public :
	FontEncoding( DictReader& self ) ;

	std::wstring Decode( const std::string& bytes ) const ;
	std::size_t Encode(
		std::wstring::const_iterator first,
		std::wstring::const_iterator last,
		std::ostream& out ) const ;

	wchar_t ToUnicode( unsigned short char_code ) const ;
	unsigned short FromUnicode( wchar_t unicode ) const ;

private :
	typedef	boost::bimap<
		boost::bimaps::unordered_set_of<unsigned short>,
		boost::bimaps::unordered_set_of<wchar_t>
	> CharMap ; 

	/// mapping from character code to unicode
	CharMap	m_charmap ;
} ;

} // end of namespace

#endif // FONTENCODING_HH_
