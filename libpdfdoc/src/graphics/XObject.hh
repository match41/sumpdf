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

/**	\file	XObject.hh
    \brief	definition the XObject class
    \date	May 19, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_XOBJECT_HH_EADER_INCLUDED__
#define __PDF_XOBJECT_HH_EADER_INCLUDED__

#include "util/RefCounter.hh"

namespace pdf {

class Graphics ;
class Matrix ;
class GraphicsState ;
class Stream ;

///	brief description
/**	\internal
	The XObject class represents
*/
class XObject : public RefCounter
{
public :
	virtual Graphics* CreateRenderedObject(
		const GraphicsState&	gs,
		const Matrix&			ctm ) const = 0 ;
} ;

XObject* CreateXObject( Stream& s ) ;

} // end of namespace

#endif // XOBJECT_HH_
