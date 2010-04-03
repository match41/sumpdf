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

/**	\file	Path.hh
    \brief	definition the Path class
    \date	Apr 3, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_PATH_HH_EADER_INCLUDED__
#define __PDF_PATH_HH_EADER_INCLUDED__

#include "Graphics.hh"

#include <cstddef>

namespace pdf {

///	brief description
/**	\internal
	The Path class represents
*/
class Path : public Graphics
{
public :
	struct Segment
	{
		double	*points ;
		enum Op { move, line, cubic123, cubic23, cubic13, close } ;
	} ;

public :
	virtual ~Path( ) ;
	
	virtual std::size_t SegmentCount( ) const = 0 ;
	virtual Segment GetSegment( std::size_t index ) const = 0 ;
} ;

} // end of namespace

#endif // PATH_HH_
