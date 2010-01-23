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
    \file	PageContent.hh
    \brief	definition the PageContent class
    \date	Dec 31, 2009
    \author	Nestal Wan
*/

#ifndef __PDF_PAGECONTENT_HEADER_INCLUDED__
#define __PDF_PAGECONTENT_HEADER_INCLUDED__

#include <vector>

namespace pdf {

class Graphics ;
class GraphicsVisitor ;
class Text ;
class TextState ;

/**	\brief	brief description

	The PageContent class represent
*/
class PageContent
{
protected :
	~PageContent( ) ;

public :
	virtual std::size_t Count( ) const = 0 ;
	virtual const Graphics* Item( std::size_t idx ) const = 0 ;
	
	virtual Text* AddText( const TextState& ts ) = 0 ;
	
	virtual void VisitGraphics( GraphicsVisitor *visitor ) = 0 ;
} ;

} // end of namespace

#endif // PAGECONTENT_HH_
