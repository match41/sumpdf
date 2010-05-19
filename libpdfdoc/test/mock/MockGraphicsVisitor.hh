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

/**	\file	MockGraphicsVisitor.hh
    \brief	definition the MockGraphicsVisitor class
    \date	Apr 6, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_MOCKGRAPHICSVISITOR_HH_EADER_INCLUDED__
#define __PDF_MOCKGRAPHICSVISITOR_HH_EADER_INCLUDED__

#include "graphics/GraphicsVisitor.hh"

namespace pdf {

///	brief description
/**	\internal
	The MockGraphicsVisitor class represents
*/
class MockGraphicsVisitor : public GraphicsVisitor
{
public :
	MockGraphicsVisitor( ) ;

	void VisitText( Text *text ) ;
	void VisitGraphics( Graphics *text ) ;
	void VisitPath( Path *path ) ;
	void VisitRenderedObject( RenderedObject<Image> *img ) ;
} ;

} // end of namespace

#endif // MOCKGRAPHICSVISITOR_HH_
