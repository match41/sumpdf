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

/**	\file	Image.hh
    \brief	definition the Image class
    \date	May 17, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_IMAGE_HH_EADER_INCLUDED__
#define __PDF_IMAGE_HH_EADER_INCLUDED__

#include <iosfwd>
#include <vector>

namespace pdf {

class Name ;
class Object ;
class Stream ;

///	brief description
/**	\internal
	The Image class represents
*/
class Image
{
public :
	Image( ) ;
	Image( Stream& str ) ;
	
	std::size_t Width() const ;
	std::size_t Height() const ;
	
	std::istream& ReadInlineImage( std::istream& is ) ;

private :
	void ProcessDictEntry( const Name& name, const Object& entry ) ;

private :
	std::size_t		m_width ;
	std::size_t		m_height ;
	
	std::vector<unsigned char>	m_bytes ;
} ;

} // end of namespace

#endif // IMAGE_HH_
