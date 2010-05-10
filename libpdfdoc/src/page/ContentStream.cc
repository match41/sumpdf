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
#include "core/TokenSrc.hh"
#include "graphics/GraphicsState.hh"
#include "graphics/GraphicsVisitor.hh"
#include "graphics/RealPath.hh"
#include "graphics/RealText.hh"
#include "stream/Stream.hh"

#include "util/Debug.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>

namespace pdf {

struct ContentStream::HandlerMap
{
	/// command handler
	typedef void (ContentStream::*Handler)( ContentOp& ) ;
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
	
	// inline image commands
//	std::make_pair( "BI",	&ContentStream::OnInlineImage ),
} ;


const ContentStream::HandlerMap::Map ContentStream::HandlerMap::m_map(
    Begin( ContentStream::HandlerMap::m_val ),
    End( ContentStream::HandlerMap::m_val ) ) ;

/**	constructor
	
*/
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
	
	std::istream s( str.InStreamBuf() ) ;
	TokenSrc src( s ) ;
	std::vector<Object> args ;
	
	ContentOp		op ;
	GraphicsState	gstate ;

	while ( src >> op )
	{
//std::cout << op  ;
		ProcessCommand( op ) ;
	}
}

void ContentStream::ProcessCommand( ContentOp& op )
{
	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( op.Operator() );
	if ( i != HandlerMap::m_map.end() )
	{
		PDF_ASSERT( i->second != 0 ) ;
		(this->*(i->second))( op ) ;
	}
	else if ( m_current != 0 )
		m_current->OnCommand( op, m_res ) ;
	else
		m_state.gs.OnCommand( op, m_res ) ;
}

void ContentStream::OnBT( ContentOp& )
{
	if ( m_current == 0 )
		m_current = new RealText( m_state.gs, m_state.ctm ) ;
}

void ContentStream::OnEndObject( ContentOp& )
{
	if ( m_current != 0 )
	{
		m_state.gs = m_current->GetState( ) ;
		
		m_current->Visit( m_visitor ) ;
		delete m_current ;
		m_current = 0 ;
	}
}

void ContentStream::Oncm( ContentOp& op )
{
	if ( op.Count() >= 6 )
		m_state.ctm = Matrix( op[0], op[1], op[2], op[3], op[4], op[5] ) * m_state.ctm ;
}

void ContentStream::OnQ( ContentOp& )
{
	m_state = m_state_stack.top( ) ;
	m_state_stack.pop( ) ;
}

void ContentStream::Onq( ContentOp& )
{
	m_state_stack.push( m_state ) ;
}

void ContentStream::Onm( ContentOp& op )
{
	if ( m_current == 0 )
		m_current = new RealPath( m_state.gs, m_state.ctm ) ;

	m_current->OnCommand( op, m_res ) ;
}

void ContentStream::OnPaintPath( ContentOp& op )
{
	if ( m_current != 0 )
		m_current->OnCommand( op, m_res ) ;
	OnEndObject( op ) ;
}

class Image : public Graphics
{
public :
	void OnCommand( ContentOp& op, const ResourcesDict *res )
	{
	}

	Matrix Transform( ) const
	{
		return m_transform ;
	}
	
	void Transform( const Matrix& mat )
	{
		m_transform = mat ;
	}

	void Print(
		std::ostream&	os,
		ResourcesDict	*res,
		GraphicsState&	gs ) const
	{
	}

	void Visit( GraphicsVisitor *visitor )
	{
	}
	
	GraphicsState GetState( ) const
	{
		return m_gs ;
	}

private :
	Matrix			m_transform ;
	GraphicsState	m_gs ;
} ;

class InlineImage : public Image
{
} ;

void ContentStream::OnInlineImage( ContentOp& op )
{
}

} // end of namespace
