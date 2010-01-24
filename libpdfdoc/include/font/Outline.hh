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

/**	\file	Outline.hh
    \brief	definition the Outline class
    \date	Jan 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_OUTLINE_HH_EADER_INCLUDED__
#define __PDF_OUTLINE_HH_EADER_INCLUDED__

namespace pdf {

///	brief description
/**	The Outline class represents
*/
class Outline
{
public :
	virtual void MoveTo( int x, int y ) = 0 ;
	virtual void LineTo( int x, int y ) = 0 ;
	virtual void QuadTo( int cx, int cy, int tx, int ty ) = 0 ;
	virtual void CubicTo(
		int c1x, int c1y,
		int c2x, int c2y,
		int tx,  int ty ) = 0 ;
} ;

} // end of namespace

#endif // OUTLINE_HH_
