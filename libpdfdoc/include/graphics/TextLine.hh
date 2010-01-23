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
#include "TextState.hh"
#include "util/Matrix.hh"

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace pdf {

class RealResources ;

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
	explicit TextLine(
		const Matrix&		transform	= Matrix(),
		const TextState& 	state		= TextState() ) ;

	// uses default generated copy constructor

	iterator begin() ;
	iterator end() ;
	
	const_iterator begin() const ;
	const_iterator end() const ;

	TextBlock& front() ;
	TextBlock& back() ;
	const TextBlock& front() const ;
	const TextBlock& back() const ;

	void AddBlock( const TextBlock& blk ) ;

	const Matrix& Transform() const ;
	void SetTransform( const Matrix& t ) ;
	bool IsEmpty( ) const ;

	void AppendText( const std::wstring& text ) ;
	void ChangeState( const TextState& s ) ;

	std::ostream& Print(
		std::ostream& 	os,
		TextState& 		state,
		Resources		*res ) const ;

	bool operator==( const TextLine& rhs ) const ;
	bool operator!=( const TextLine& rhs ) const ;

private :
	/// the text matrix
	Matrix	m_trans ;
	
	std::vector<TextBlock>	m_blks ;
} ;

} // end of namespace

#endif // TEXTLINE_HH_
