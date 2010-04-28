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
	TextState		text ;
	
	Color			strk_color, fill_color ;
	
	double			line_width ;
	PenCapStyle		pen_cap ;
	LineJoinStyle	line_join ;
	double			miter_limit ;
	
	std::vector<int>	dash_pattern ;
	int					dash_phase ;

	// association with external states dictionaries
	Name		name ;

	Impl( )
		: line_width( 0 )
		, pen_cap( butt_cap )
		, line_join( miter_join )
		, miter_limit( 0 )
		, dash_phase( 0 )
	{
	}
	
	Impl( const TextState& ts )
		: text( ts )
		, line_width( 0 )
		, pen_cap( butt_cap )
		, line_join( miter_join )
		, miter_limit( 0 )
		, dash_phase( 0 )
	{
	}

	template <typename T>
	bool PrintField(
		const GraphicsState::Impl&		prev,
		T GraphicsState::Impl::*		field,
		const std::string&				op,
		std::ostream&					os ) ;
} ;

template <>
bool GraphicsState::Impl::PrintField(
	const GraphicsState::Impl&		prev,
	Color GraphicsState::Impl::*	field,
	const std::string&				op,
	std::ostream&					os )
{
	if ( this->*field != prev.*field )
	{
		std::copy( (this->*field).begin(), (this->*field).end(),
			std::ostream_iterator<double>( os, " " ) ) ;
		os << op << '\n' ;
		return true ;
	}
	else
		return false ;
}

template <typename T>
bool GraphicsState::Impl::PrintField(
	const GraphicsState::Impl&	prev,
	T GraphicsState::Impl::*	field,
	const std::string&			op,
	std::ostream&				os )
{
	if ( this->*field != prev.*field )
	{
		os << this->*field << ' ' << op << '\n' ;
		return true ;
	}
	return false ;
}

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

const TextState& GraphicsState::Text() const
{
	return m_impl->text ;
}

TextState& GraphicsState::Text()
{
	CopyOnWrite( ) ;
	return m_impl->text ;
}

void GraphicsState::PrintColors(
	std::ostream&			os,
	ResourcesDict			*res,
	const GraphicsState&	prev ) const
{
	Color::Space ssp = m_impl->strk_color.ColorSpace() ;
	Color::Space fsp = m_impl->fill_color.ColorSpace() ;

	PDF_ASSERT( ssp >= Color::rgb && ssp <= Color::cmyk ) ;
	PDF_ASSERT( fsp >= Color::rgb && fsp <= Color::cmyk ) ;

	static const std::string strk_ops[]	= { "RG", "G", "K" } ;
	static const std::string fill_ops[]	= { "rg", "g", "k" } ;
	
	m_impl->PrintField( *prev.m_impl, &Impl::strk_color, strk_ops[ssp], os ) ;
	m_impl->PrintField( *prev.m_impl, &Impl::fill_color, fill_ops[fsp], os ) ;
}

std::ostream& GraphicsState::Print(
	std::ostream&			os,
	ResourcesDict			*res,
	const GraphicsState&	prev ) const
{
	// print text state
	m_impl->text.Print( os, res, prev.m_impl->text ) ;
	
	PrintColors( os, res, prev ) ;
	m_impl->PrintField( *prev.m_impl, &Impl::line_width,	"w", os ) ;
	m_impl->PrintField( *prev.m_impl, &Impl::pen_cap,		"J", os ) ;
	m_impl->PrintField( *prev.m_impl, &Impl::line_join,		"j", os ) ;
	m_impl->PrintField( *prev.m_impl, &Impl::miter_limit,	"M", os ) ;
	
	if ( m_impl->dash_pattern	!= prev.m_impl->dash_pattern ||
		 m_impl->dash_phase		!= prev.m_impl->dash_phase )
	{
		os 	<< Object(m_impl->dash_pattern) << ' ' << m_impl->dash_phase
			<< " d\n" ; 
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
				 m_impl->text.FontFace()	!= f )
			{
				CopyOnWrite( ) ;
				m_impl->text.ChangeFont( font_size, f ) ;
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
		m_impl->text.Leading( op[0].To<double>() ) ;
		return true ;
	}
	else
		return false ;
}

Font* GraphicsState::FontFace( ) const
{
	return m_impl->text.FontFace( ) ;
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
	return os << gs.m_impl->text << " " << gs.StrokeColor() << " " << gs.FillColor() ;
}

bool GraphicsState::LineWidth( double value )
{
	if ( m_impl->line_width != value )
	{
		CopyOnWrite( ) ;
		m_impl->line_width = value ;
		return true ;
	}
	else
		return false ;
}

double GraphicsState::LineWidth( ) const
{
	return m_impl->line_width ;
}

bool GraphicsState::PenCap( PenCapStyle value )
{
	PDF_ASSERT( value >= butt_cap ) ;
	PDF_ASSERT( value <= project_cap ) ;

	if ( m_impl->pen_cap != value )
	{
		CopyOnWrite( ) ;
		m_impl->pen_cap = value ;
		return true ;
	}
	else
		return false ;
}

GraphicsState::PenCapStyle GraphicsState::PenCap( ) const
{
	PDF_ASSERT( m_impl->pen_cap >= butt_cap ) ;
	PDF_ASSERT( m_impl->pen_cap <= project_cap ) ;
	
	return m_impl->pen_cap ;
}

bool GraphicsState::LineJoin( LineJoinStyle value )
{
	PDF_ASSERT( value >= miter_join ) ;
	PDF_ASSERT( value <= bevel_join ) ;

	if ( m_impl->line_join != value )
	{
		CopyOnWrite( ) ;
		m_impl->line_join = value ;
		return true ;
	}
	else
		return false ;
}

GraphicsState::LineJoinStyle GraphicsState::LineJoin( ) const
{
	PDF_ASSERT( m_impl->line_join >= miter_join ) ;
	PDF_ASSERT( m_impl->line_join <= bevel_join ) ;

	return m_impl->line_join ;
}

bool GraphicsState::MiterLimit( double value )
{
	if ( m_impl->miter_limit != value )
	{
		CopyOnWrite( ) ;
		m_impl->miter_limit = value ;
		return true ;
	}
	else
		return false ;
}

double GraphicsState::MiterLimit( ) const
{
	return m_impl->miter_limit ;
}

void GraphicsState::SetValue( const Name& name, const Object& val )
{
	CopyOnWrite( ) ;
	if ( name == "LW" )
		m_impl->line_width = static_cast<LineJoinStyle>(val.To<int>() )  ;
	else if ( name == "LC" )
		m_impl->pen_cap = static_cast<PenCapStyle>(val.To<int>()) ;
}

const Color& GraphicsState::StrokeColor( ) const
{
	return m_impl->strk_color ;
}

const Color& GraphicsState::FillColor( ) const
{
	return m_impl->fill_color ;
}

bool GraphicsState::OnCS( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 1 ?
		SetColorSpace( strk_color, op[0].As<Name>() ) :
		false ;
}

bool GraphicsState::StrokeColor( const Color& color )
{
	return ChangeColor( strk_color, color ) ;
}

bool GraphicsState::FillColor( const Color& color )
{
	return ChangeColor( fill_color, color ) ;
}

bool GraphicsState::ChangeColor( ColorType type, const Color& color )
{
	Color Impl::*colors[] = { &Impl::strk_color, &Impl::fill_color } ;
	
	if ( m_impl.get()->*colors[type] != color )
	{
		// m_impl will be changed after calling CopyOnWrite()
		CopyOnWrite( ) ;
		
		m_impl.get()->*colors[type] = color ;
		return true ;
	}
	else
		return false ;
}

bool GraphicsState::SetColorSpace( ColorType type, const Name& cs )
{
	Color temp ;
	if ( cs == "DeviceGray" )
		temp.AssignGray( 0.0 ) ;
	else if ( cs == "DeviceRGB" )
		temp.AssignRGB( 0.0, 0.0, 0.0 ) ;
	else if ( cs == "DeviceCMYK" )
		temp.AssignCMYK( 0.0, 0.0, 0.0, 0.0 ) ;

	return ChangeColor( type, temp ) ;
}

bool GraphicsState::Oncs( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 1 ?
		SetColorSpace( fill_color, op[0].As<Name>() ) :
		false ;
}

bool GraphicsState::OnG( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 1 ?
		ChangeColor( strk_color, Color( op[0].To<double>() ) ):
		false ;
}

bool GraphicsState::Ong( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 1 ?
		ChangeColor( fill_color, Color( op[0].To<double>() ) ):
		false ;
}

bool GraphicsState::OnRG( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 3 ?
		ChangeColor( strk_color, Color( op[0], op[1], op[2] ) ) :
		false ;
}

bool GraphicsState::Onrg( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 3 ?
		ChangeColor( fill_color, Color( op[0], op[1], op[2] )):
		false ;
}

bool GraphicsState::OnK( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 4 ?
		ChangeColor( strk_color, Color( op[0], op[1], op[2], op[3] ) ) :
		false ;
}

bool GraphicsState::Onk( ContentOp& op, const ResourcesDict *res )
{
	return op.Count() >= 4 ?
		ChangeColor( fill_color, Color( op[0], op[1], op[2], op[3] ) ) :
		false ;
}

bool GraphicsState::Onw( ContentOp& op, const ResourcesDict * )
{
	if ( op.Count() >= 1 )
	{
		CopyOnWrite( ) ;
		m_impl->line_width = op[0] ;
		return true ;
	}
	return false ;
}

bool GraphicsState::OnJ( ContentOp& op, const ResourcesDict *res )
{
	if ( op.Count() >= 1 )
	{
		CopyOnWrite( ) ;
		m_impl->pen_cap = static_cast<PenCapStyle>( op[0].To<int>() ) ;
		return true ;
	}
	return false ;
}

bool GraphicsState::Onj( ContentOp& op, const ResourcesDict *res )
{
	if ( op.Count() >= 1 )
	{
		CopyOnWrite( ) ;
		m_impl->line_join = static_cast<LineJoinStyle>( op[0].To<int>() ) ;
		return true ;
	}
	return false ;
}

bool GraphicsState::OnM( ContentOp& op, const ResourcesDict *res )
{
	if ( op.Count() >= 1 )
	{
		CopyOnWrite( ) ;
		m_impl->miter_limit = op[0].To<double>() ;
		return true ;
	}
	return false ;
}

bool GraphicsState::Ond( ContentOp& op, const ResourcesDict *res )
{
	if ( op.Count() >= 2 )
	{
		CopyOnWrite( ) ;
		m_impl->dash_pattern	= op[0].To<std::vector<int> >() ;
		m_impl->dash_phase		= op[1].To<int>() ;
		return true ;
	}
	return false ;
}

std::vector<int> GraphicsState::DashPattern() const
{
	return m_impl->dash_pattern ;
}

int GraphicsState::DashPhrase( ) const
{
	return m_impl->dash_phase ;
}

} // end of namespace
