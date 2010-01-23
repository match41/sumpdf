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
    \file	Graphics.hh
    \brief	definition the Graphics class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GRAPHICS_HH_EADER_INCLUDED__
#define __PDF_GRAPHICS_HH_EADER_INCLUDED__

#include <cstddef>
#include <iosfwd>

namespace pdf {

class Token ;
class Resources ;
class GraphicsVisitor ;

/**	\addtogroup	graphics Graphics
	\brief	Graphics objects module
	\internal
	
	The graphics objects represent visual elements that appears in the page.
	These classes provide access to their content.
*/

/**	\brief	brief description

	The Graphics class represent base class for anything that appears in the
	page.
*/
class Graphics
{
public :
	virtual ~Graphics( ) ;
	
	virtual void Print( std::ostream& os, Resources *res ) const = 0 ;

	virtual void Visit( GraphicsVisitor *visitor ) = 0 ;
} ;

} // end of namespace

#endif // GRAPHICS_HH_
