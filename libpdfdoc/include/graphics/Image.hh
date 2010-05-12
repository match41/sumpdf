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

/**	\file	Image.hh
    \brief	definition the Image class
    \date	May 11, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_IMAGE_HEADER_INCLUDED__
#define __PDF_IMAGE_HEADER_INCLUDED__

#include "Graphics.hh"

#include "GraphicsState.hh"
#include "util/Matrix.hh"

namespace pdf {

///	brief description
/**	The Image class represents
*/
class Image : public Graphics
{
public :
	Image( ) ;
	~Image( ) ;

	void OnCommand( ContentOp& op, const ResourcesDict *res ) ;

	Matrix Transform( ) const ;
	void Transform( const Matrix& mat ) ;
	void Print(
		std::ostream&	os,
		ResourcesDict	*res,
		GraphicsState&	gs ) const ;
	void Visit( GraphicsVisitor *visitor ) ;
	
	GraphicsState GetState( ) const ;

private :
	Matrix			m_transform ;
	GraphicsState	m_gs ;
} ;

} // end of namespace

#endif // IMAGE_HH_
