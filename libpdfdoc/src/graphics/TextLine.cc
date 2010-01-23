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

/**
	\file	TextLine.cc
	\brief	implementation of the TextLine class
	\date	Jan 4, 2010
	\author	Nestal Wan
*/

#include "graphics/TextLine.hh"

#include "core/Object.hh"
#include "core/Token.hh"
#include "font/Font.hh"

#include "util/Util.hh"
#include "util/Exception.hh"

#include <cassert>
#include <set>

#include <iostream>

namespace pdf {

///	constructor
TextLine::TextLine( const Matrix& transform, const TextState& state )
    : m_trans( transform ),
      m_blks( 1, TextBlock( std::string(), state ) )
{
}

void TextLine::AddBlock( const TextBlock& blk )
{
	m_blks.push_back( blk ) ;
}

TextLine::iterator TextLine::begin()
{
	return m_blks.begin( ) ;
}

TextLine::iterator TextLine::end()
{
	return m_blks.end( ) ;
}

TextLine::const_iterator TextLine::begin() const
{
	return m_blks.begin( ) ;
}

TextLine::const_iterator TextLine::end() const
{
	return m_blks.end( ) ;
}

const Matrix& TextLine::Transform() const
{
	return m_trans ;
}

void TextLine::SetTransform( const Matrix& t )
{
	m_trans = t ;
}

bool TextLine::IsEmpty( ) const
{
	return m_blks.size() == 1 && m_blks.front().IsEmpty() ;
}

void TextLine::AppendText( const std::wstring& text )
{
	Font *f = m_blks.back().Format().GetFont() ;
	if ( f == 0 )
		throw Exception( "invalid font" ) ;
	else
	{
//		std::cout << "using font: " << f->BaseName() << std::endl ; 
		m_blks.back().AppendText( text ) ;
	}
}

std::ostream& TextLine::Print(
	std::ostream& 	os,
	TextState& 		state,
	Resources		*res ) const
{
	const Matrix& t = m_trans ;
	if ( t.M11() == 1 && t.M12() == 0 &&
		 t.M21() == 0 && t.M22() == 1 )
		os << "Td " << t.Dx() << ' ' << t.Dy() << '\n' ;
	else
		os << "Tm " << t.M11() << ' ' << t.M12() << ' ' << t.M21() << ' '
		            << t.M22() << ' ' << t.Dx()  << ' ' << t.Dy( ) << '\n' ; 
	
	for ( TextLine::const_iterator i = m_blks.begin() ;
		i != m_blks.end() ; ++i )
	{
		i->Print( os, state, res ) ;
		state = i->Format( ) ;
	}
	
	return os ;
}

void TextLine::ChangeState( const TextState& s )
{
	if ( m_blks.back().IsEmpty() )
		m_blks.back().SetFormat( s ) ;
	else
		m_blks.push_back( TextBlock( "", s ) ) ;
}

} // end of namespace
