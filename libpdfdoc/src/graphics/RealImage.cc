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
#include "graphics/ExtGraphicsLink.hh"
#include "graphics/RealColorSpace.hh"
#include "file/DictReader.hh"
#include "file/File.hh"
#include "stream/Stream.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <iostream>
#include <map>

namespace pdf {

/**	constructor
	
*/
RealImage::RealImage( Stream& str, File *file )
	: m_is_jpeg( str.FilterName() != Name() )
{
	try
	{
		Init( str.Self(), file ) ;
		str.CopyData( m_bytes ) ;
	}
	catch ( Exception& e )
	{
		e.Add( "Exception thrown for RealImage" ) ;
		throw ;
	}
}

void RealImage::Init( Dictionary& dict, File *file )
{
	DictReader dr( dict, file ) ;
	
	m_space = dr.Create( "ColorSpace",
		boost::bind( NewPtr<RealColorSpace>(), _1, file ) ) ;

	if (!dr.Detach( "Width", 			m_width )	||
		!dr.Detach( "Height",			m_height )	||
		!dr.Detach( "BitsPerComponent",	m_depth )	)
		throw Exception( "invalid image without width or height" ) ;
}

RealImage::RealImage( std::istream& is )
	: m_width( 0 )
	, m_height( 0 )
{
	Dictionary dict ;
	
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
						<< m_height << "\n" << dict << std::endl ;
						Init( dict, 0 ) ;
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
			{
				if ( value.Is<Name>() )
					value = ExpandAbbv( value.As<Name>() ) ;
					
				dict.insert( ExpandAbbv(key.As<Name>()), value ) ;
			}
		}
	}
	std::cout << "premature finish" << std::endl ;
}

RealImage::~RealImage( )
{
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
	return new ExtGraphicsLink<Image>( gs, ctm, this ) ;
}

ColorSpace*	RealImage::Space( ) const
{
	return m_space.get() ;
}

std::size_t RealImage::ByteCount() const
{
	return m_bytes.size() ;
}

const unsigned char* RealImage::Pixels() const
{
	return m_bytes.empty() ? 0 : &m_bytes[0] ;
}

bool RealImage::IsJpeg( ) const
{
	return m_is_jpeg ;
}

Name RealImage::ExpandAbbv( const Name& name )
{
	typedef std::map<Name, Name> NameMap ;
	static const NameMap::value_type map_val[] =
	{
		std::make_pair(Name("BPC"),	Name("BitsPerComponent") ),
		std::make_pair(Name("CS"),	Name("ColorSpace") ),
		std::make_pair(Name("D"),	Name("Decode") ),
		std::make_pair(Name("DP"),	Name("DecodeParms") ),
		std::make_pair(Name("F"),	Name("Filter") ),
		std::make_pair(Name("H"),	Name("Height") ),
		std::make_pair(Name("IM"),	Name("ImageMask") ),
		std::make_pair(Name("I"),	Name("Interpolate") ),
		std::make_pair(Name("W"),	Name("Width") ),
		std::make_pair(Name("G"),	Name("DeviceGray") ),
		std::make_pair(Name("RGB"),	Name("DeviceRGB") ),
		std::make_pair(Name("CMYK"),Name("DeviceCMYK") ),
		std::make_pair(Name("I"),	Name("Indexed") ),
		std::make_pair(Name("AHx"),	Name("ASCIIHexDecode") ),
		std::make_pair(Name("A85"),	Name("ASCII85Decode") ),
		std::make_pair(Name("LZW"),	Name("LZWDecode") ),
		std::make_pair(Name("Fl"),	Name("FlateDecode") ),
		std::make_pair(Name("RL"),	Name("RunLengthDecode") ),
		std::make_pair(Name("CCF"),	Name("CCITTFaxDecode") ),
		std::make_pair(Name("DCT"),	Name("DCTDecode") ),
	} ;
	
	static const NameMap mp( Begin(map_val), End(map_val) ) ;
	NameMap::const_iterator i = mp.find( name ) ;
	return i != mp.end() ? i->second : name ;
}

} // end of namespace
