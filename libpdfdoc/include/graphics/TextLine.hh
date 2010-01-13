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

#include <iosfwd>
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
	typedef std::vector<TextBlock>::iterator		iterator ;
	typedef std::vector<TextBlock>::const_iterator	const_iterator ;

public :
	TextLine( ) ;

	// uses default generated copy constructor

	void OnCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t		count,
		Resources		*res ) ;

	iterator begin() ;
	iterator end() ;
	
	const_iterator begin() const ;
	const_iterator end() const ;

	void AddBlock( const TextBlock& blk ) ;

	const Matrix& Transform() const ;
	bool IsEmpty( ) const ;

	friend std::ostream& operator<<( std::ostream& os, const TextLine& line ) ;

private :
	void OnTd( Object* args, std::size_t count, Resources *res ) ;
	void OnTD( Object* args, std::size_t count, Resources *res ) ;
	void OnTm( Object* args, std::size_t count, Resources *res ) ;
	void OnTstar( Object* args, std::size_t count, Resources *res ) ;

	///	Encapsulate the command handler mapping.
	struct Map ;
	friend struct Map ;

	void NewBlock( ) ;

private :
	/// the text matrix
	Matrix	m_trans ;
	
	std::vector<TextBlock>	m_blks ;
} ;

} // end of namespace

#endif // TEXTLINE_HH_
