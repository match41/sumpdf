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

#include "util/Util.hh"

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
	m_blks.back().AppendText( text ) ;
}

std::ostream& operator<<( std::ostream& os, const TextLine& line )
{
	const Matrix& t = line.m_trans ;
	if ( t.M11() == 1 && t.M12() == 0 &&
		 t.M21() == 0 && t.M22() == 1 )
		os << "Td " << t.Dx() << ' ' << t.Dy() << '\n' ;
	else
		os << "Tm " << t.M11() << ' ' << t.M12() << ' ' << t.M21() << ' '
		            << t.M22() << ' ' << t.Dx()  << ' ' << t.Dy( ) << '\n' ; 
	
	std::copy(
		line.m_blks.begin(),
		line.m_blks.end(),
		std::ostream_iterator<TextBlock>( os ) ) ;
	
	return os ;
}

} // end of namespace
