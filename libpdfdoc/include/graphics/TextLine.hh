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

/**	\file	TextLine.hh
    \brief	definition the TextLine class
    \date	Jun 28, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTLINE_HEADER_INCLUDED__
#define __PDF_TEXTLINE_HEADER_INCLUDED__

#include <string>

namespace pdf {

class CharVisitor ;
class GraphicsState ;
class Matrix ;

///	brief description
/**	\internal
	The TextLine class represents
*/
class TextLine
{
public :
	virtual ~TextLine( ) ;

	virtual TextLine* Clone( ) const = 0 ;

	virtual const Matrix& Transform() const = 0 ;
	virtual void SetTransform( const Matrix& t ) = 0 ;
	virtual bool IsEmpty( ) const = 0 ;

	virtual void AppendText( const std::wstring& text ) = 0 ;

	virtual void AppendSpace( double width ) = 0 ;

	virtual void AppendText( double xpos, const std::wstring& text ) = 0 ;

	/// Returns the text state of the text line.
	virtual const GraphicsState& Format() const = 0 ;
	virtual void SetFormat( const GraphicsState& fmt ) = 0 ;

	///	Returns the unicode string of the text line.
	virtual const std::wstring& Text() const = 0 ;
	
	///	Returns the width of the text line in text space units.
	virtual double Width( ) const = 0 ;
	
	virtual double Height( ) const = 0 ;
	
	///	Walkthrough all characters in the text line.
	/**	This function will call CharVisitor::OnChar() for each character it
		contains. The arguments of CharVisitor::OnChar() provide information
		to render the character glyphs.
		\param	v	An implementation of the CharVisitor interface. Its
					OnChar() member function will be called.
	*/
	virtual void VisitChars( CharVisitor *v ) const = 0 ;
} ;

} // end of namespace

#endif // TEXTLINE_HH_
