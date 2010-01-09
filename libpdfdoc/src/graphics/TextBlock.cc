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

namespace pdf {

/**	constructor
*/
TextBlock::TextBlock( )
{
}

TextBlock::TextBlock( const std::string& text, const TextFormat& format )
	: m_chars( text.begin(), text.end() ),
	  m_format( format )
{
}

const TextFormat& TextBlock::Format() const
{
	return m_format ;
}

TextFormat& TextBlock::Format()
{
	return m_format ;
}

void TextBlock::OnCommand(
	const Token& 	cmd,
	const Object 	*args,
	std::size_t		count,
	Resources		*res )
{
}

bool TextBlock::IsEmpty() const
{
	return m_chars.empty() ;
}

} // end of namespace
