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

#ifndef __PDF_TEXT_HH_EADER_INCLUDED__
#define __PDF_TEXT_HH_EADER_INCLUDED__

#include "Graphics.hh"

#include <string>
#include <vector>

namespace pdf {

class TextLine ;
class GraphicsVisitor ;

///	Text objects.
/**	\ingroup graphics
	The Text class represent a PDF text object. It is the stuff enclosed by
	a BT...ET operators in the content stream of a page. It consists of a number
	of text lines.
*/
class Text : virtual public Graphics
{
public :
	typedef	std::vector<TextLine>::iterator			iterator ;
	typedef	std::vector<TextLine>::const_iterator	const_iterator ;

protected :
	~Text( ) ;

public :

	///	\name Iterator access members
	//@{
	///	Iterator access to the underlying text lines
	virtual iterator begin() = 0 ;
	virtual iterator end() = 0 ;
	
	virtual const_iterator begin() const = 0 ;
	virtual const_iterator end() const = 0 ;
	//@}
	
	virtual std::size_t Count( ) const = 0 ;
	
	///	Add a new line to the text object.
	virtual void AddLine( const TextLine& line ) = 0 ;
	
	/// Add a new line with coordinate and text.
	virtual void AddLine( double x, double y, const std::wstring& text ) = 0 ;

	///	Visitor rebound function.
	virtual void Visit( GraphicsVisitor *visitor ) = 0 ;
} ;

} // end of namespace

#endif // TEXT_HH_
