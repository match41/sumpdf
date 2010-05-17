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

/**	\file	MockGraphicsVisitor.cc
	\brief	implementation of the MockGraphicsVisitor class
	\date	Apr 6, 2010
	\author	Nestal Wan
*/

#include "MockGraphicsVisitor.hh"

namespace pdf {

/**	constructor
	
*/
MockGraphicsVisitor::MockGraphicsVisitor( )
{
}

void MockGraphicsVisitor::VisitText( Text *text )
{
}

void MockGraphicsVisitor::VisitGraphics( Graphics *text )
{
}

void MockGraphicsVisitor::VisitPath( Path *path )
{
}

void MockGraphicsVisitor::VisitImage( XObject<Image> *img )
{
}

} // end of namespace
