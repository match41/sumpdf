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

#include "graphics/XObject.hh"

#include "core/Name.hh"
#include "core/Object.hh"

namespace pdf {

template <typename T>
XObject<T>::XObject( const GraphicsState& gs, const Matrix& ctm, const T& t )
	: m_transform( ctm )
	, m_gs( gs )
	, m_obj( t )
{
}

/**	constructor
	
*/
template <typename T>
XObject<T>::~XObject( )
{
}

template <typename T>
void XObject<T>::OnCommand( ContentOp& op, const ResourcesDict *res )
{
}

template <typename T>
Matrix XObject<T>::Transform( ) const
{
	return m_transform ;
}

template <typename T>
void XObject<T>::Transform( const Matrix& mat )
{
	m_transform = mat ;
}

template <typename T>
void XObject<T>::Print(
	std::ostream&	os,
	ResourcesDict	*res,
	GraphicsState&	gs ) const
{
}

template <typename T>
void XObject<T>::Visit( GraphicsVisitor *visitor )
{
}

template <typename T>
GraphicsState XObject<T>::GetState( ) const
{
	return m_gs ;
}

template <typename T>
const T& XObject<T>::Get( ) const
{
	return m_obj ;
}

template <typename T>
T& XObject<T>::Get( )
{
	return m_obj ;
}

} // end of namespace

// explicit instantiation
namespace pdf
{
	class Image ;
	template class XObject<Image*> ;
}
