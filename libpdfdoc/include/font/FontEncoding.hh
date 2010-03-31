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
    \date	Mar 31, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTENCODING_HEADER_INCLUDED__
#define __PDF_FONTENCODING_HEADER_INCLUDED__

#include <string>
#include <iosfwd>

namespace pdf {

///	brief description
/**	\internal
	The FontEncoding class represents
*/
class FontEncoding
{
public :
	virtual ~FontEncoding() ;

	virtual std::wstring Decode( const std::string& bytes ) const = 0 ;
	virtual std::size_t Encode(
		std::wstring::const_iterator first,
		std::wstring::const_iterator last,
		std::ostream& out ) const = 0 ;
} ;

} // end of namespace

#endif // FONTENCODING_HH_
