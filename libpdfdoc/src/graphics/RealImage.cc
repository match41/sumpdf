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

/**	\file	RealImage.cc
	\brief	implementation of the RealImage class
	\date	May 19, 2010
	\author	Nestal Wan
*/

#include "RealImage.hh"

#include "core/Name.hh"
#include "core/Object.hh"
#include "graphics/RenderedObject.hh"
#include "stream/Stream.hh"

#include <iostream>

namespace pdf {

/**	constructor
	
*/
RealImage::RealImage( Stream& str )
{
	std::cout << "image? " << str.Self() << std::endl ;
}

RealImage::RealImage( std::istream& is )
	: m_width( 0 )
	, m_height( 0 )
{
	Object key ;
	while ( is >> key )
	{
		if ( key.Is<Token>() && key.As<Token>().Get() == "ID" )
		{
std::cout << "got ID" << std::endl ;
			while ( is )
			{
				int ich = is.rdbuf()->sgetc() ;
				if ( ich == std::istream::traits_type::eof() )
				{
					std::cout << "EOF!" << std::endl ;
					return ;
				}

				char ch = std::istream::traits_type::to_char_type(ich) ;

				if ( ch == 'E' )
				{
					is.rdbuf()->sbumpc() ;
					
					int ich2 = is.rdbuf()->sgetc() ;
					if ( ich2 == std::istream::traits_type::eof() )
					{
						std::cout << "EOF!" << std::endl ;
						return ;
					}
					
					if ( std::istream::traits_type::to_char_type(ich2) == 'I' )
					{
						std::cout << "finished inline image" << std::endl ;
						std::cout << "width = " << m_width << " height = "
						<< m_height << std::endl ;
						return ;
					}
				}
				
				m_bytes.push_back( ch ) ;
				is.rdbuf()->sbumpc() ;
			}
		}
		else if ( key.Is<Name>() )
		{
			Object value ;
			if ( is >> value )
				ProcessDictEntry( key.As<Name>(), value ) ;
		}
	}
	std::cout << "premature finish" << std::endl ;
}

void RealImage::ProcessDictEntry( const Name& name, const Object& entry )
{
	if ( name == "W" || name == "Width" )
		m_width = entry ;
	else if ( name == "H" || name == "Height" )
		m_height = entry ;
}

std::size_t RealImage::Width( ) const
{
	return m_width ;
}

std::size_t RealImage::Height( ) const
{
	return m_height ;
}

Graphics* RealImage::CreateRenderedObject(
	const GraphicsState&	gs,
	const Matrix&			ctm ) const
{
	return new RenderedObject<Image>( gs, ctm, this ) ;
}

} // end of namespace
