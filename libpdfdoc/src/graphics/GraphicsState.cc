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

#include "core/Token.hh"

#include "util/Util.hh"

#include <set>

namespace pdf {

/**	constructor
	
*/
GraphicsState::GraphicsState( const TextState& ts )
	: m_text( ts )
{
}

const TextState& GraphicsState::GetTextState() const
{
	return m_text ;
}

std::ostream& GraphicsState::Print(
	std::ostream&			os,
	const Resources			*res,
	const GraphicsState&	prev ) const
{
	m_text.Print( os, res, prev.m_text ) ;
	return os ;
}

void GraphicsState::OnCommand(
	const Token& 	cmd,
	Object 			*args,
	std::size_t		count,
	Resources		*res )
{
}

bool GraphicsState::IsGSCommand( const Token& cmd )
{
	static const std::string cmds[] =
	{
		"w", "J", "j", "M", "d", "ri", "i", "gs"
		"q", "Q", "cm",
		
		"Tc", "Tw", "Tz", "TL", "Tf", "Tr", "Ts",
	} ;
	static const std::set<std::string> cmd_set( Begin(cmds), End(cmds) ) ;
	return cmd_set.find( cmd.Get() ) != cmd_set.end() ;
}

} // end of namespace
