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

class ColorSpace ;

///	An image.
/**	\ingroup graphics
	The Image class represents
*/
class Image
{
public :
	virtual ~Image( ) ;
	
	virtual std::size_t Width( ) const = 0 ;
	virtual std::size_t Height( ) const = 0 ;

	virtual ColorSpace*	Space( ) const = 0 ;

	virtual std::size_t ByteCount() const = 0 ;
	virtual const unsigned char* Pixels() const = 0 ;
	
	virtual bool IsJpeg( ) const = 0 ;
} ;

} // end of namespace

#endif // IMAGE_HH_
