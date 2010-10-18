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

/**	\file	GraphicsImpl.hh
    \brief	definition the GraphicsImpl class
    \date	Oct 17, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GRAPHICSIMPL_HH_EADER_INCLUDED__
#define __PDF_GRAPHICSIMPL_HH_EADER_INCLUDED__

#include "graphics/Graphics.hh"

#include <iosfwd>

namespace pdf {

class ContentOp ;
class GraphicsState ;
class ResourcesDict ;

///	Implementation detail of a Graphics object.
/**	\internal
	Basically, all graphics object must implement this interface. This interface
	contains functions that should be part of the Graphics interface. However,
	these functions are not suppose to be expose to the clients of the library.
	As a result, they are moved to this interface.
*/
class GraphicsImpl : virtual public Graphics
{
public :
	virtual void OnCommand( ContentOp& op, const ResourcesDict *res ) = 0 ;

	virtual void Print(
		std::ostream&	os,
		ResourcesDict	*res,
		GraphicsState&	gs ) const = 0 ;
} ;

} // end of namespace

#endif // GRAPHICSIMPL_HH_
