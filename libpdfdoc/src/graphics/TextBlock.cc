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
	\file	TextBlock.cc
	\brief	implementation of the TextBlock class
	\date	Jan 4, 2010
	\author	Nestal Wan
*/

#include "graphics/TextBlock.hh"

#include "core/Array.hh"
#include "core/Object.hh"
#include "core/Token.hh"

#include <iostream>

namespace pdf {

///	constructor
TextBlock::TextBlock( )
{
}

TextBlock::TextBlock( const std::string& text, const TextState& format )
	: m_chars( text.begin(), text.end() ),
	  m_format( format )
{
}

const TextState& TextBlock::Format() const
{
	return m_format ;
}

void TextBlock::SetFormat( const TextState& fmt )
{
	m_format = fmt ;
}

bool TextBlock::IsEmpty() const
{
	return m_chars.empty() ;
}

const std::wstring& TextBlock::Text() const
{
	return m_chars ;
}

void TextBlock::SetText( const std::wstring& text )
{
	m_chars = text ;
}

void TextBlock::AppendText( const std::wstring& text )
{
	m_chars.insert( m_chars.end(), text.begin(), text.end() ) ;
}

} // end of namespace
