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

class PathSegment ;
class Matrix ;

///	An abstraction of a shape, trajectory and region of all sorts.
/**	\ingroup graphics
	The Path class represents shapes, trajectories, and regions of all sorts.
	They are used to draw lines, define the shapes of filled areas.
*/
class Path : virtual public Graphics
{
public :
	enum	FillMode { winding, odd_even } ;

public :
	virtual ~Path( ) ;
	
	/// Returns the number of segment in the path
	virtual std::size_t Count( ) const = 0 ;
	
	/// Returns the segment for the specified index. 
	virtual PathSegment Segment( std::size_t index ) const = 0 ;
	
	virtual Matrix Transform( ) const = 0 ;
	virtual void Transform( const Matrix& mat ) = 0 ;
	
	virtual void AddSegment( const PathSegment& seg ) = 0 ;
	virtual void MoveTo( double x, double y ) = 0 ;
	virtual void LineTo( double x, double y ) = 0 ;
	virtual void CloseSubPath( ) = 0 ;
	virtual bool IsFill( ) const = 0 ;
	virtual bool IsStroke( ) const = 0 ;
	virtual FillMode FillRule( ) const = 0 ;
	virtual bool SetStyle( bool fill, bool stroke, FillMode fill_mode ) = 0 ;
} ;

} // end of namespace

#endif // PATH_HH_
