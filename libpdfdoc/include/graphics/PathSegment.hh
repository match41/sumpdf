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

/**	\file	PathSegment.hh
    \brief	definition the PathSegment class
    \date	Apr 6, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_PATHSEGMENT_HH_EADER_INCLUDED__
#define __PDF_PATHSEGMENT_HH_EADER_INCLUDED__

#include <cstddef>

namespace pdf {

///	brief description
/**	\internal
	The PathSegment class represents
*/
class PathSegment
{
public :
	enum Op { move, line, cubic123, cubic23, cubic13, close } ;

public :
	explicit PathSegment( Op op = close, const double *points = 0 ) ;

	// default copy constructor is fine.

	const double* Points( ) const ;
	std::size_t Count( ) const ;
	Op GetOp( ) const ;

private :
	const double	*m_points ;
	Op				m_op ;
} ;

} // end of namespace

#endif // PATHSEGMENT_HH_
