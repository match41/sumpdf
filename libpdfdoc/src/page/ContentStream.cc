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

/**	\file	ContentStream.cc
	\brief	implementation of the ContentStream class
	\date	Feb 20, 2010
	\author	Nestal Wan
*/

#include "ContentStream.hh"

#include "ContentOp.hh"
#include "ResourcesDict.hh"

#include "graphics/GraphicsState.hh"
#include "graphics/GraphicsVisitor.hh"
#include "graphics/RealImage.hh"
#include "graphics/RealPath.hh"
#include "graphics/RealText.hh"
#include "graphics/ExtGraphicsLink.hh"
#include "graphics/TextState.hh"
#include "graphics/XObject.hh"
#include "stream/Stream.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>

#include <ctime>

namespace pdf {

class Clock
{
private :
	const std::clock_t m_start ;

public :
	Clock( ) : m_start( std::clock() ) {}
	~Clock( )
	{
		std::clock_t diff = std::clock() - m_start ;
		std::cout << "ticks = " << (double)diff/CLOCKS_PER_SEC << std::endl ;
	}
} ;

struct ContentStream::HandlerMap
{
	/// command handler
	typedef void (ContentStream::*Handler)( ContentOp&, std::istream& ) ;
	typedef std::map<Token, Handler>	Map ;

	static const Map::value_type	m_val[] ;
	static const Map				m_map ;
} ;

const ContentStream::HandlerMap::Map::value_type
	ContentStream::HandlerMap::m_val[] =
{
	// text object commands
	std::make_pair( "BT",	&ContentStream::OnBT ),
	std::make_pair( "ET",	&ContentStream::OnEndObject ),
	
	// special graphics commands
	std::make_pair( "cm",	&ContentStream::Oncm ),
	std::make_pair( "Q",	&ContentStream::OnQ ),
	std::make_pair( "q",	&ContentStream::Onq ),
	
	// path construction & painting commands
	std::make_pair( "m",	&ContentStream::Onm ),
	std::make_pair( "re",	&ContentStream::Onm ),
	std::make_pair( "S",	&ContentStream::OnPaintPath ),
	std::make_pair( "s",	&ContentStream::OnPaintPath ),
	std::make_pair( "f",	&ContentStream::OnPaintPath ),
	std::make_pair( "F",	&ContentStream::OnPaintPath ),
	std::make_pair( "f*",	&ContentStream::OnPaintPath ),
	std::make_pair( "B",	&ContentStream::OnPaintPath ),
	std::make_pair( "B*",	&ContentStream::OnPaintPath ),
	std::make_pair( "b",	&ContentStream::OnPaintPath ),
	std::make_pair( "b*",	&ContentStream::OnPaintPath ),
	std::make_pair( "n",	&ContentStream::OnPaintPath ),
	
	// inline image
	std::make_pair( "BI",	&ContentStream::OnInlineImage ),
	
	// XObjects
	std::make_pair( "Do",	&ContentStream::OnDoXObject ),
} ;

const ContentStream::HandlerMap::Map ContentStream::HandlerMap::m_map(
    Begin( ContentStream::HandlerMap::m_val ),
    End( ContentStream::HandlerMap::m_val ) ) ;

ContentStream::~ContentStream( )
{
	std::for_each( m_inline_imgs.begin(), m_inline_imgs.end(), DeletePtr() ) ;
}

void ContentStream::Decode( )
{
	std::for_each(
		m_strs.begin(),
		m_strs.end(),
		boost::bind( &ContentStream::Decode, this, _1 ) ) ;
}

void ContentStream::Decode( Stream& str )
{
	PDF_ASSERT( m_res != 0 ) ;
	PDF_ASSERT( m_current == 0 ) ;

	// rewind to stream start for reading
	str.Rewind( ) ;
	
	std::istream src( str.InStreamBuf() ) ;
	std::vector<Object> args ;
	
	ContentOp	op ;

	while ( src >> op )
	{
		ProcessCommand( op, src ) ;
	}
}

void ContentStream::ProcessCommand( ContentOp& op, std::istream& is )
{
	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( op.Operator() );
	if ( i != HandlerMap::m_map.end() )
	{
		PDF_ASSERT( i->second != 0 ) ;
		(this->*(i->second))( op, is ) ;
	}
	else if ( m_current != 0 )
		m_current->OnCommand( op, m_res ) ;
	else
		m_state.gs.OnCommand( op, m_res ) ;
}

void ContentStream::OnBT( ContentOp&, std::istream& )
{
	if ( m_current == 0 )
		m_current = new RealText( m_state.gs, m_state.ctm ) ;
}

void ContentStream::OnEndObject( ContentOp&, std::istream& )
{
	if ( m_current != 0 )
	{
		m_state.gs = m_current->GetState( ) ;
		
		m_current->Visit( m_visitor ) ;
		delete m_current ;
		m_current = 0 ;
	}
}

void ContentStream::Oncm( ContentOp& op, std::istream& )
{
	if ( op.Count() >= 6 )
		m_state.ctm = Matrix( op[0], op[1], op[2], op[3], op[4], op[5] ) *
			m_state.ctm ;
}

void ContentStream::OnQ( ContentOp&, std::istream& )
{
	std::cout << "Q1 font = " << (void*)m_state.gs.Text().FontFace() << std::endl ;

	m_state = m_state_stack.top( ) ;
	m_state_stack.pop( ) ;

	std::cout << "Q2 font = " << (void*)m_state.gs.Text().FontFace() << std::endl ;
}

void ContentStream::Onq( ContentOp&, std::istream& )
{
	std::cout << "q1 font = " << (void*)m_state.gs.Text().FontFace() << std::endl ;
	m_state_stack.push( m_state ) ;
	std::cout << "q2 font = " << (void*)m_state.gs.Text().FontFace() << std::endl ;
}

void ContentStream::Onm( ContentOp& op, std::istream& )
{
	if ( m_current == 0 )
		m_current = new RealPath( m_state.gs, m_state.ctm ) ;

	m_current->OnCommand( op, m_res ) ;
}

void ContentStream::OnPaintPath( ContentOp& op, std::istream& is )
{
	if ( m_current != 0 )
		m_current->OnCommand( op, m_res ) ;
	OnEndObject( op, is ) ;
}

void ContentStream::OnInlineImage( ContentOp& op, std::istream& is )
{
	Image *img = new RealImage( is ) ;
	m_inline_imgs.push_back( img ) ;
	
	// m_current will be deleted in OnEndObject()
	m_current = new ExtGraphicsLink<Image>( m_state.gs, m_state.ctm, img ) ;
	OnEndObject( op, is ) ;
	
	PDF_ASSERT( m_current == 0 ) ;
}

std::vector<Image*> ContentStream::InlineImages( ) const
{
	return m_inline_imgs ;
}

void ContentStream::OnDoXObject( ContentOp& op, std::istream& is )
{
	if ( op.Count() >= 1 )
	{
		XObject *xo = m_res->FindXObject( op[0].As<Name>() ) ;
		if ( xo != 0 )
		{
			m_current = xo->CreateLink( m_state.gs, m_state.ctm ) ;
			OnEndObject( op, is ) ;
		}
	}
}

} // end of namespace
