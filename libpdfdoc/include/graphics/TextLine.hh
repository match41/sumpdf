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

#include "libpdfdoc.hh"

#include "GraphicsState.hh"
#include "util/Matrix.hh"

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace pdf {

class CharVisitor ;

class RealResources ;

///	brief description
/**	\ingroup graphics

	The TextLine class represent a unit of text. Within a text line, the text
	matrix (e.g. position) and the text state will not change. In other words,
	the text line contains the text, matrix and the text state. 
*/
class LIBPDFDOC_API TextLine
{
public :
	explicit TextLine(
		double	xpos = 0, double ypos = 0,
		
		const GraphicsState& 	state		= GraphicsState(),		
		const Matrix&			transform	= Matrix(),
		const std::wstring&		text		= std::wstring() ) ;

	// uses default generated copy constructor

	double XPos( ) const ;
	double YPos( ) const ;

	const Matrix& Transform() const ;
	void SetTransform( const Matrix& t ) ;
	bool IsEmpty( ) const ;

	void AppendText( const std::wstring& text ) ;

	void AppendSpace( double width ) ;

	std::ostream& Print(
		std::ostream& 			os,
		Matrix&					current,
		const GraphicsState& 	state,
		const Resources			*res ) const ;

	bool operator==( const TextLine& rhs ) const ;
	bool operator!=( const TextLine& rhs ) const ;

	/// Returns the text state of the text line.
	const GraphicsState& Format() const ;
	void SetFormat( const GraphicsState& fmt ) ;

	///	Returns the unicode string of the text line.
	const std::wstring& Text() const ;
	
	///	Returns the width of the text line in text space units.
	double Width( ) const ;
	
	///	Walkthrough all characters in the text line.
	/**	This function will call CharVisitor::OnChar() for each character it
		contains. The arguments of CharVisitor::OnChar() provide information
		to render the character glyphs.
		\param	v	An implementation of the CharVisitor interface. Its
					OnChar() member function will be called.
	*/
	void VisitChars( CharVisitor *v ) const ;
	
	friend std::ostream& operator<<( std::ostream& os, const TextLine& t ) ;

private :
	std::ostream& PrintText( std::ostream& os ) const ;

private :
	double	m_xpos, m_ypos ;

	/// The text matrix.
	Matrix			m_trans ;
	
	///	The text state.
	GraphicsState	m_state ;
	
	///	The text string.
	std::wstring	m_text ;

	///	Represent a space inside the text line.
	/**	The TJ command can be used to specify a horizontal (or vertical, 
		depending on text direction) displacement of current text base point.
	*/
	struct Space
	{
		std::size_t	index ;
		double		width ;
	} ;
	
	///	A collection of spaces.
	/**	In this vector, the index field will be monotonically increasing to
		allow fast binary search.
	*/
	std::vector<Space>	m_space ;
} ;

} // end of namespace

#endif // TEXTLINE_HH_
