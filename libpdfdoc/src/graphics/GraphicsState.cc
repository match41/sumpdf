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

/**	\file	GraphicsState.cc
	\brief	implementation of the GraphicsState class
	\date	Feb 16, 2010
	\author	Nestal Wan
*/

#include "graphics/GraphicsState.hh"

#include "graphics/Color.hh"
#include "graphics/TextState.hh"

#include "core/Object.hh"
#include "core/Token.hh"

#include "font/BaseFont.hh"

#include "page/ContentOp.hh"
#include "page/ResourcesDict.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/Matrix.hh"
#include "util/Util.hh"

#include <boost/format.hpp>

#include <map>
#include <iostream>

namespace pdf {

struct GraphicsState::HandlerMap
{
	/// command handler
	typedef bool (GraphicsState::*Handler)( ContentOp&, const ResourcesDict* ) ;
	typedef std::map<Token, Handler>	Map ;

	static const Map::value_type	m_val[] ;
	static const Map				m_map ;
} ;

const GraphicsState::HandlerMap::Map::value_type
	GraphicsState::HandlerMap::m_val[] =
{
	// text state commands
	std::make_pair( "Tf",	&GraphicsState::OnTf ),
	std::make_pair( "TL",	&GraphicsState::OnTL ),

	// colour space commands
	std::make_pair( "CS",	&GraphicsState::OnCS ),
	std::make_pair( "cs",	&GraphicsState::Oncs ),
	std::make_pair( "G",	&GraphicsState::OnG ),
	std::make_pair( "g",	&GraphicsState::Ong ),
	std::make_pair( "RG",	&GraphicsState::OnRG ),
	std::make_pair( "rg",	&GraphicsState::Onrg ),
	std::make_pair( "K",	&GraphicsState::OnK ),
	std::make_pair( "k",	&GraphicsState::Onk ),

	// graphics states commands
	std::make_pair( "w",	&GraphicsState::Onw ),
	std::make_pair( "J",	&GraphicsState::OnJ ),
	std::make_pair( "j",	&GraphicsState::Onj ),
	std::make_pair( "M",	&GraphicsState::OnM ),
	std::make_pair( "d",	&GraphicsState::Ond ),
} ;

const GraphicsState::HandlerMap::Map GraphicsState::HandlerMap::m_map(
    Begin( GraphicsState::HandlerMap::m_val ),
    End( GraphicsState::HandlerMap::m_val ) ) ;

struct GraphicsState::Impl
{
	TextState	text ;
	
	Color		strk_colour, non_strk_colour ;
	
	double		line_width ;
	PenCap		pen_cap ;
	LineJoin	line_join ;
	double		miter_limit ;

	// association with external states dictionaries
	Name		name ;

	Impl( )
	{
	}
	
	Impl( const TextState& ts )
	: text( ts )
	{
	}
} ;

GraphicsState::GraphicsState( )
	: m_impl( new Impl )
{
}

GraphicsState::GraphicsState( const TextState& ts )
	: m_impl( new Impl( ts ) )
{
}

GraphicsState::~GraphicsState( )
{
}

void GraphicsState::CopyOnWrite( )
{
	if ( m_impl.use_count() > 1 )
		m_impl.reset( new Impl( *m_impl ) ) ;
}

const TextState& GraphicsState::GetTextState() const
{
	return m_impl->text ;
}

TextState& GraphicsState::GetTextState()
{
	CopyOnWrite( ) ;
	return m_impl->text ;
}

std::ostream& GraphicsState::Print(
	std::ostream&			os,
	ResourcesDict			*res,
	const GraphicsState&	prev ) const
{
	m_impl->text.Print( os, res, prev.m_impl->text ) ;
	if ( m_impl->strk_colour != prev.m_impl->strk_colour )
	{
		const Color& strk = m_impl->strk_colour ;
		std::copy( strk.begin(), strk.end(),
			std::ostream_iterator<double>( os, " " ) ) ;
		
		switch ( strk.ColorSpace() )
		{
			// there will be a space after copying to ostream_iterator
			case Color::rgb:	os << "RG\n" ; break ;
			case Color::gray:	os << "G\n" ; break ;
			case Color::cmyk:	os << "K\n" ; break ;
		}
	}
	if ( m_impl->non_strk_colour != prev.m_impl->non_strk_colour )
	{
		const Color& nstrk = m_impl->non_strk_colour ;
		std::copy( nstrk.begin(), nstrk.end(),
			std::ostream_iterator<double>( os, " " ) ) ;
		
		switch ( nstrk.ColorSpace() )
		{
			// there will be a space after copying to ostream_iterator
			case Color::rgb:	os << "rg\n" ; break ;
			case Color::gray:	os << "g\n" ; break ;
			case Color::cmyk:	os << "k\n" ; break ;
		}
	}
	return os ;
}

/**	Handles PDF operators in the content stream.
	\return	\c true if the state is changed by the command. Otherwise \c false.
*/
bool GraphicsState::OnCommand( ContentOp& op, const ResourcesDict *res )
{
	try
	{
		HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( op.Operator() );
		if ( i != HandlerMap::m_map.end() )
			return (this->*(i->second))( op, res ) ;
	
		return false ;
	}
	catch ( Exception& e )
	{
		e.Add( boost::format( "Error when parsing \"%1%\"" ) % op ) ;
		throw ;
	}
}

bool GraphicsState::IsGSCommand( const Token& cmd )
{
	HandlerMap::Map::const_iterator i = HandlerMap::m_map.find( cmd ) ;
	return i != HandlerMap::m_map.end() ;
}

bool GraphicsState::OnTf( ContentOp& op, const ResourcesDict *res )
{
	PDF_ASSERT( res != 0 ) ;

	if ( op.Count() >= 2 && op[0].Is<Name>() && op[1].IsNumber() )
	{
		BaseFont *f = res->FindFont( op[0].As<Name>() ) ;
		if ( f != 0 )
		{
			double font_size = op[1].To<double>() ;
			
			if ( m_impl->text.FontSize()	!= font_size	||
				m_impl->text.GetFont()	!= f )
			{
				CopyOnWrite( ) ;
				m_impl->text.SetFont( font_size, f ) ;
				return true ;
			}
		}
	}
	return false ;
}

bool GraphicsState::OnTL( ContentOp& op, const ResourcesDict *res )
{
	if ( op.Count() > 0 && op[0].IsNumber() )
	{
		CopyOnWrite( ) ;
		m_impl->text.SetLeading( op[0].To<double>() ) ;
		return true ;
	}
	else
		return false ;
}

Font* GraphicsState::GetFont( ) const
{
	return m_impl->text.GetFont( ) ;
}

bool GraphicsState::operator==( const GraphicsState& gs ) const
{
	// TODO: add more members
	return m_impl->text == gs.m_impl->text ;
}

bool GraphicsState::operator!=( const GraphicsState& gs ) const
{
	return !operator==( gs ) ;
}

std::ostream& operator<<( std::ostream& os, const GraphicsState& gs )
{
	return os << gs.m_impl->text ;
}

void GraphicsState::LineWidth( double value )
{
	CopyOnWrite( ) ;
	m_impl->line_width = value ;
}

double GraphicsState::LineWidth( ) const
{
	return m_impl->line_width ;
}

void GraphicsState::SetPenCap( PenCap value )
{
	CopyOnWrite( ) ;
	m_impl->pen_cap = value ;
}

GraphicsState::PenCap GraphicsState::GetPenCap( ) const
{
	return m_impl->pen_cap ;
}

void GraphicsState::SetLineJoin( LineJoin value )
{
	CopyOnWrite( ) ;
	m_impl->line_join = value ;
}

GraphicsState::LineJoin GraphicsState::GetLineJoin( ) const
{
	return m_impl->line_join ;
}

void GraphicsState::MiterLimit( double value )
{
	CopyOnWrite( ) ;
	m_impl->miter_limit = value ;
}

double GraphicsState::MiterLimit( ) const
{
	return m_impl->miter_limit ;
}

void GraphicsState::SetValue( const Name& name, const Object& val )
{
	CopyOnWrite( ) ;
	if ( name == "LW" )
		m_impl->line_width = static_cast<LineJoin>(val.To<int>() )  ;
	else if ( name == "LC" )
		m_impl->pen_cap = static_cast<PenCap>(val.To<int>()) ;
}

const Color& GraphicsState::StrokeColour( ) const
{
	return m_impl->strk_colour ;
}

const Color& GraphicsState::NonStrokeColour( ) const
{
	return m_impl->non_strk_colour ;
}

bool GraphicsState::OnCS( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 1 ?
		SetColourSpace( m_impl->strk_colour, op[0].As<Name>() ) :
		false ;
}

bool GraphicsState::ChangeColour( Color& old, const Color& new_ )
{
	if ( old != new_ )
	{
		old = new_ ;
		return true ;
	}
	else
		return false ;
}

bool GraphicsState::SetColourSpace( Color& colour, const Name& cs )
{
	Color temp ;
	if ( cs == "DeviceGray" )
		temp.AssignGray( 0.0 ) ;
	else if ( cs == "DeviceRGB" )
		temp.AssignRGB( 0.0, 0.0, 0.0 ) ;
	else if ( cs == "DeviceCMYK" )
		temp.AssignCMYK( 0.0, 0.0, 0.0, 0.0 ) ;

	return ChangeColour( colour, temp ) ;
}

bool GraphicsState::Oncs( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 1 ?
		SetColourSpace( m_impl->non_strk_colour, op[0].As<Name>() ) :
		false ;
}

bool GraphicsState::OnG( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 1 ?
		ChangeColour( m_impl->strk_colour, Color( op[0].To<double>() ) ):
		false ;
}

bool GraphicsState::Ong( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 1 ?
		ChangeColour( m_impl->non_strk_colour, Color( op[0].To<double>() ) ):
		false ;
}

bool GraphicsState::OnRG( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 3 ?
		ChangeColour( m_impl->strk_colour, Color( op[0], op[1], op[2] ) ) :
		false ;
}

bool GraphicsState::Onrg( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 3 ?
		ChangeColour( m_impl->non_strk_colour, Color( op[0], op[1], op[2] )):
		false ;
}

bool GraphicsState::OnK( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 4 ?
		ChangeColour( m_impl->strk_colour,
			Color( op[0], op[1], op[2], op[3] ) ) :
		false ;
}

bool GraphicsState::Onk( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 4 ?
		ChangeColour( m_impl->non_strk_colour,
			Color( op[0], op[1], op[2], op[3] ) ) :
		false ;
}

bool GraphicsState::Onw( ContentOp& op, const ResourcesDict * )
{
	if ( op.Count() >= 1 )
	{
		m_impl->line_width = op[0] ;
		return true ;
	}
	return false ;
}

bool GraphicsState::OnJ( ContentOp& op, const ResourcesDict *res )
{
	return false ;
}

bool GraphicsState::Onj( ContentOp& op, const ResourcesDict *res )
{
	return false ;
}

bool GraphicsState::OnM( ContentOp& op, const ResourcesDict *res )
{
	return false ;
}

bool GraphicsState::Ond( ContentOp& op, const ResourcesDict *res )
{
	return false ;
}

} // end of namespace
