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

/**	\file	GfxParamDict.hh
    \brief	definition the GfxParamDict class
    \date	Feb 25, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GFXPARAMDICT_HH_EADER_INCLUDED__
#define __PDF_GFXPARAMDICT_HH_EADER_INCLUDED__

#include "graphics/GraphicsState.hh"

namespace pdf {

class DictReader ;
class File ;
class Ref ;

///	Graphics state parameter dictionary
/**	\internal
	This class represents a graphics state parameter dictionary in the
	resources of a page or page node. See p. 219 of the PDF specification.
*/
class GfxParamDict
{
public :
	explicit GfxParamDict( const GraphicsState& state = GraphicsState() ) ;

	void Read( DictReader& dict ) ;
	Ref Write( File *file ) const ;

private :
	GraphicsState	m_state ;
} ;

} // end of namespace

#endif // GFXPARAMDICT_HH_
