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
    \file	Text.hh
    \brief	definition the Text class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXT_HEADER_INCLUDED__
#define __PDF_TEXT_HEADER_INCLUDED__

#include "Graphics.hh"

#include <string>
#include <vector>

namespace pdf {

class TextLine ;
class GraphicsVisitor ;

///	Text objects.
/**	\ingroup graphics
	The Text class represent a PDF text object. A text object in a page contains
	a group of text string with different formats. There is no limit on how
	much text can be included in a Text object. It is completely up to the
	application which generated the PDF file.
	
	A Text object can contain multiple TextLine objects. Each TextLine object
	represent a number of characters with the same format (i.e. TextState) and
	position (i.e. transformation). As mentioned above, the PDF generator
	application defines how to group TextLine in Text objects. Technically, a
	Text object contains all the stuff enclosed by a BT...ET operators in the
	content stream of a page.
	
	The order of the TextLine in a Text object will not affect their absolute
	position in the page. It will only affect the order of rendering. In
	general, lines defined first will be rendered first. In other words,
	objects rendered subsequently may be overwritten by later objects if they
	are overlapped.
*/
class Text : public Graphics
{
public :
	virtual ~Text( ) ;

	///	\name Access to TextLine 
	/**	These functions provide random access to the underlying text lines. The
		order of the TextLine will be the same as their appearance in the PDF
		file.
	*/
	//@{
	/// Random access to text lines. 
	virtual const TextLine* At( std::size_t idx ) const = 0 ;
	
	/// Random access to text lines.
	virtual TextLine* At( std::size_t idx ) = 0 ;
	
	///	Return the number of TextLine objects in the Text object.
	virtual std::size_t Count( ) const = 0 ;
	//@}
	
	///	Add a new line to the text object.
	/**	This function adds a new line to the text object. The newly added line
		will be appended to the end of the text object.
	*/
	virtual void AddLine( const TextLine *line ) = 0 ;
	
	/// Add a new line with coordinate and text.
	virtual void AddLine( double x, double y, const std::wstring& text ) = 0 ;

	///	Visitor rebound function.
	virtual void Visit( GraphicsVisitor *visitor ) = 0 ;
} ;

} // end of namespace

#endif // TEXT_HH_
