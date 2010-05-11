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

/**	\file	Image.cc
	\brief	implementation of the Image class
	\date	May 11, 2010
	\author	Nestal Wan
*/

#include "graphics/Image.hh"

namespace pdf {

/**	constructor
	
*/
Image::~Image( )
{
}

void Image::OnCommand( ContentOp& op, const ResourcesDict *res )
{
}

Matrix Image::Transform( ) const
{
	return m_transform ;
}

void Image::Transform( const Matrix& mat )
{
	m_transform = mat ;
}

void Image::Print(
	std::ostream&	os,
	ResourcesDict	*res,
	GraphicsState&	gs ) const
{
}

void Image::Visit( GraphicsVisitor *visitor )
{
}

GraphicsState Image::GetState( ) const
{
	return m_gs ;
}

} // end of namespace
