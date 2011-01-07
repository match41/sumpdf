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

/**	\file	RealGraphicsGroup.hh
    \brief	definition the RealGraphicsGroup class
    \date	May 23, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALGRAPHICSGROUP_HH_EADER_INCLUDED__
#define __PDF_REALGRAPHICSGROUP_HH_EADER_INCLUDED__

#include "graphics/GraphicsGroup.hh"
#include "XObject.hh"

namespace pdf {

class Stream ;
class File ;

///	brief description
/**	\internal
	The RealGraphicsGroup class represents
*/
class RealGraphicsGroup : public GraphicsGroup, public XObject
{
public :
	RealGraphicsGroup( Stream&, File * ) ;

	Ref Write( File *file ) const ;

	std::size_t Count( ) const ;
	const Graphics* At( std::size_t idx ) const ;

	Graphics* CreateLink(
		const GraphicsState&	gs,
		const Matrix&			ctm ) const ;
} ;

} // end of namespace

#endif // REALGRAPHICSGROUP_HH_
