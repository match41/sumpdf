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
    \file	TextLine.hh
    \brief	definition the TextLine class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTLINE_HEADER_INCLUDED__
#define __PDF_TEXTLINE_HEADER_INCLUDED__

#include "TextBlock.hh"

#include "util/Matrix.hh"

#include <map>
#include <vector>

namespace pdf {

class Resources ;

/**	\brief	brief description

	The TextLine class represent a line of text. Inside the line, the text
	matrix (e.g. position) will not change. A line of text contains a list
	of text blocks. Each block will have a string of characters with the same
	formatting parameters.
*/
class TextLine
{
public :
	TextLine( ) ;

	void OnCommand(
		const Token& 	cmd,
		const Object 	*args,
		std::size_t		count,
		Resources		*res ) ;

private :
	void OnTd( const Object* args, std::size_t count, Resources *res ) ;
	void OnTD( const Object* args, std::size_t count, Resources *res ) ;
	void OnTm( const Object* args, std::size_t count, Resources *res ) ;
	void OnTstar( const Object* args, std::size_t count, Resources *res ) ;

	typedef void (TextLine::*Handler)(
		const Object	*args,
		std::size_t		count,
		Resources		*res ) ;
	typedef std::map<Token, Handler>	HandlerMap ;

	static const HandlerMap::value_type	m_handler_map_values[] ;
	static const HandlerMap				m_handler_map ;

	void NewBlock( ) ;

private :
	/// the text matrix
	Matrix	m_trans ;
	
	std::vector<TextBlock>	m_blks ;
} ;

} // end of namespace

#endif // TEXTLINE_HH_
