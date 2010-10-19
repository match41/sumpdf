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

#include "graphics/ExtGraphicsLink.hh"
#include "graphics/GraphicsVisitor.hh"

#include "core/Name.hh"
#include "core/Object.hh"

#include "util/Debug.hh"

namespace pdf {

template <typename T>
ExtGraphicsLink<T>::ExtGraphicsLink(
	const GraphicsState&	gs,
	const Matrix&			ctm,
	const T*				t )
	: m_transform( ctm )
	, m_gs( gs )
	, m_obj( t )
{
}

/**	constructor
	
*/
template <typename T>
ExtGraphicsLink<T>::~ExtGraphicsLink( )
{
}

template <typename T>
void ExtGraphicsLink<T>::OnCommand( ContentOp& op, const ResourcesDict *res )
{
}

template <typename T>
Matrix ExtGraphicsLink<T>::Transform( ) const
{
	return m_transform ;
}

template <typename T>
void ExtGraphicsLink<T>::Transform( const Matrix& mat )
{
	m_transform = mat ;
}

template <typename T>
void ExtGraphicsLink<T>::Print(
	std::ostream&	os,
	ResourcesDict	*res,
	GraphicsState&	gs ) const
{
}

template <typename T>
void ExtGraphicsLink<T>::Visit( GraphicsVisitor *visitor )
{
	// assume the GraphicsVisitor has an overloaded function for each
	// rendered object (e.g. image) type.
	visitor->VisitGraphicsLink( this ) ;
}

template <typename T>
GraphicsState ExtGraphicsLink<T>::GetState( ) const
{
	return m_gs ;
}

template <typename T>
const T* ExtGraphicsLink<T>::Get( ) const
{
	return m_obj ;
}

template <typename T>
GraphicsImpl* ExtGraphicsLink<T>::QueryImplementation( )
{
	return this ;
}

template <typename T>
const GraphicsImpl* ExtGraphicsLink<T>::QueryImplementation( ) const
{
	return this ;
}

template <typename T>
Graphics* ExtGraphicsLink<T>::Interface( )
{
	return this ;
}

} // end of namespace

// explicit instantiation
namespace pdf
{
	class Image ;
	template class ExtGraphicsLink<Image> ;
	template class ObjectAppearance<Image> ;
	
	class GraphicsGroup ;
	template class ExtGraphicsLink<GraphicsGroup> ;
	template class ObjectAppearance<GraphicsGroup> ;
}
