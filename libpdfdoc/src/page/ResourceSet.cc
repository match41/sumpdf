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

/**	\file	ResourceSet.cc
	\brief	implementation of the ResourcesSet class
	\date	May 21, 2010
	\author	Nestal Wan
*/

#include "ResourceSet.hh"

namespace pdf {

/**	constructor
	
*/
template <typename T>
ResourceSet<T>::ResourceSet( )
{
}

template <typename T>
template <typename Factory>
void ResourceSet<T>::Read( DictReader& self, Factory f )
{
}

template <typename T>
Name ResourceSet<T>::Find( const T *t ) const
{
	return Name() ;
}

template <typename T>
T* ResourceSet<T>::Find( const Name& name )
{
	return 0 ;
}

template <typename T>
const T* ResourceSet<T>::Find( const Name& name ) const
{
	return 0 ;
}

} // end of namespace

#include "font/BaseFont.hh"

namespace pdf
{
	template class ResourceSet<BaseFont> ;
	template void ResourceSet<BaseFont>::Read( DictReader& self, int f ) ;
}
