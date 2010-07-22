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

/**	\file	RealImage.hh
    \brief	definition the RealImage class
    \date	May 19, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REAL_IMAGE_HEADER_INCLUDED__
#define __PDF_REAL_IMAGE_HEADER_INCLUDED__

#include "graphics/Image.hh"
#include "XObject.hh"

#include "util/RefPtr.hh"

#include <vector>
#include <iosfwd>

namespace pdf {

class RealColorSpace ;
class Dictionary ;
class Name ;
class Object ;
class Stream ;

///	brief description
/**	\internal
	The RealImage class represents
*/
class RealImage : public Image, public XObject
{
public :
	explicit RealImage( std::istream& is ) ;
	RealImage( Stream& str, File *file ) ;
	~RealImage( ) ;

	std::size_t Width( ) const ;
	std::size_t Height( ) const ;

	std::istream& ReadInlineImage( std::istream& is ) ;

	Graphics* CreateLink(
		const GraphicsState&	gs,
		const Matrix&			ctm ) const ;

	ColorSpace*	Space( ) const ;
	std::size_t Depth( ) const ;

	std::size_t ByteCount() const ;
	const unsigned char* Pixels() const ;

	bool IsJpeg( ) const ;

private :
	void Init( Dictionary& dict, File *file ) ;

	static Name ExpandAbbv( const Name& name ) ;

	void ReadContent( Dictionary& dict, std::istream& is ) ;

private :
	std::size_t		m_width ;
	std::size_t		m_height ;
	std::size_t		m_depth ;
	
	std::vector<unsigned char>				m_bytes ;
	boost::intrusive_ptr<RealColorSpace>	m_space ;
	
	bool			m_is_jpeg ;
} ;

} // end of namespace

#endif // REALIMAGE_HH_
