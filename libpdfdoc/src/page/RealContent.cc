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

/**	\file	RealContent.cc
	\brief	implementation of the RealContent class
	\date	Jan 14, 2010
	\author	Nestal Wan
*/

#include "RealContent.hh"

#include "core/Object.hh"
#include "core/Token.hh"
#include "core/TokenSrc.hh"
#include "graphics/GraphicsVisitor.hh"
#include "graphics/RealText.hh"
#include "stream/Stream.hh"

#include <boost/bind.hpp>

#include <cassert>
#include <iostream>
#include <sstream>

namespace pdf {

/**	constructor
*/
RealContent::RealContent( )
{
}

void RealContent::Add( Graphics *gfx )
{
	assert( gfx != 0 ) ;
	m_gfx.push_back( gfx ) ;
}

///	Decode the page contents.
/**	This function will decode the content stream of the page and create
	Graphics objects. The newly created Graphics will be stored in the
	vector in the Content object.
*/
void RealContent::Load( Stream& str, Resources *res )
{
	// rewind to stream start for reading
	str.Rewind( ) ;
	
	std::istream s( str.InStreamBuf() ) ;
	TokenSrc src( s ) ;
	std::vector<Object> args ;

	Graphics *current = 0 ;

	while ( true )
	{
		Token  cmd ;
		Object obj ;

		if ( src >> obj )
		{
			// swapping is faster
			args.push_back( Object() ) ;
			obj.Swap( args.back() ) ;
		}
		
		// if it is not an object, then it should be a command operator
		else
		{
			src.ResetState( ) ;
			if ( src >> cmd )
			{
//std::cout << cmd.Get() << " " ;
//std::copy( args.begin(), args.end(), std::ostream_iterator<Object>( std::cout, " " ) ) ;
//std::cout << std::endl ;
				current = ProcessCommand(
					cmd,
					args.empty() ? 0 : &args[0],	// don't touch args[0]
					args.size(),					// if empty.
					current,
					res) ;

				args.clear( ) ;
			}
			else
				break ;
		}
	}
}

Graphics* RealContent::ProcessCommand(
	const Token& 	cmd,
	Object 			*args,
	std::size_t 	count,
	Graphics		*gfx,
	Resources 		*res  )
{
	assert( count > 0 || args == 0 ) ;

	if ( cmd == Token("BT") && gfx == 0 )
	{
		gfx = new RealText ;
	}
	else if ( cmd == Token("ET") && gfx != 0 )
	{
		m_gfx.push_back( gfx ) ;
		gfx = 0 ;
	}
	else if ( gfx != 0 )
	{
		// TODO: remove dynamic_cast
		RealText *text = dynamic_cast<RealText*>( gfx ) ;
		text->OnCommand( cmd, args, count, res ) ;
	}
	
	return gfx ;
}

bool RealContent::IsEmpty( ) const
{
	return m_gfx.empty() ;
}

std::size_t RealContent::Count( ) const
{
	return m_gfx.size( ) ;
}

const Graphics* RealContent::Item( std::size_t idx ) const
{
	return m_gfx.at( idx ) ;
}

Text* RealContent::AddText( const TextState& ts )
{
	RealText *t = new RealText( ts ) ;
	m_gfx.push_back( t ) ;
	return t ;
}

void RealContent::VisitGraphics( GraphicsVisitor *visitor )
{
	// all pointers are not null
	assert( std::find( m_gfx.begin(), m_gfx.end(), (void*)0 ) == m_gfx.end() ) ;

	std::for_each(
		m_gfx.begin(),
		m_gfx.end(),
		boost::bind( &Graphics::Visit, _1, visitor ) ) ;
}

void RealContent::Write( Stream& str, const Resources *res ) const
{
	struct ContentWriter : public GraphicsVisitor
	{
		ContentWriter( std::ostream& os, const Resources *res  )
			: m_os(os), m_res( res )
		{
		}
		
		void VisitText( Text *text )
		{
			text->Print( m_os, m_res ) ; 
		}
		
		void VisitGraphics( Graphics *text )
		{
		}
		
		std::ostream&		m_os ;
		const Resources	*m_res ;
	} ;
	
	std::ostream os( str.OutStreamBuf() ) ;
	ContentWriter cw( os, res ) ;
	(const_cast<RealContent*>(this))->VisitGraphics( &cw ) ;
	os.flush() ;
}

} // end of namespace
