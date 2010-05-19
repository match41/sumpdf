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
    \file	GraphicsVisitor.hh
    \brief	definition the GraphicsVisitor class
    \date	Jan 9, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GRAPHICSVISITOR_HEADER_INCLUDED__
#define __PDF_GRAPHICSVISITOR_HEADER_INCLUDED__

namespace pdf {

class Graphics ;
class Image ;
class Path ;
class Text ;

template <typename T> class RenderedObject ;

///	brief description
/**	\ingroup graphics
	The GraphicsVisitor class represent
*/
class GraphicsVisitor
{
protected :
	~GraphicsVisitor( ) ;

public :
	virtual void VisitText( Text *text ) = 0 ;
	virtual void VisitGraphics( Graphics *text ) = 0 ;
	virtual void VisitPath( Path *path ) = 0 ;
	virtual void VisitRenderedObject( RenderedObject<Image> *img ) = 0 ;
} ;

} // end of namespace

#endif // GRAPHICSVISITOR_HH_
