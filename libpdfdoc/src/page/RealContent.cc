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
#include "graphics/GraphicsState.hh"
#include "graphics/GraphicsVisitor.hh"
#include "graphics/RealText.hh"
#include "graphics/TextState.hh"
#include "stream/Stream.hh"

#include <boost/bind.hpp>

#include <algorithm>
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
	RealText *t = new RealText( GraphicsState(ts) ) ;
	m_gfx.push_back( t ) ;
	return t ;
}

Text* RealContent::AddText( double size, Font *font )
{
	return AddText( TextState( size, font ) ) ;
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
	std::ostream os( str.OutStreamBuf() ) ;

	using namespace boost ;
	std::for_each(
		m_gfx.begin(),
		m_gfx.end(),
		bind( &Graphics::Print, _1, ref(os), res ) ) ;

	os.flush() ;
}

/// Remove all contents.
void RealContent::Clear( )
{
	m_gfx.clear( ) ;
}

} // end of namespace
