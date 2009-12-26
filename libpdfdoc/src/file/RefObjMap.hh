/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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

/**
	\file	ResourcePool.hh
	\brief	definition the ResourcePool class
	\date	Dec 23, 2009
	\author	Nestal Wan
*/


#ifndef __PDF_REFOBJMAP_HH_EADER_INCLUDED__
#define __PDF_REFOBJMAP_HH_EADER_INCLUDED__

#include "core/Ref.hh"

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

#include <string>
#include <cassert>

namespace pdf {

/**	\brief	mapping for Ref and resources
	when reading a PDF file, the PDF resources may be mapped indirectly using
	references. Multiple references may refer to the same resource. This is to
	identifies them and share them. 
*/
template <typename T>
class RefObjMap
{
public :
	RefObjMap( )
	{
	}
	
	void Add( const Ref& link, T *res )
	{
		assert( m_pool.left.find(link) == m_pool.left.end() ) ;
		m_pool.insert( MapEntry( link, res ) ) ;
	}

	T* Find( const Ref& link )
	{
		typename MapType::left_iterator i = m_pool.left.find( link ) ;
		if ( i != m_pool.left.end() )
		{
			assert( i->second != 0 ) ;
		
			// assume T is a RefCountObj
			i->second->AddRef( ) ;
			return i->second ;
		}
		else
			return 0 ;
	}

	Ref Find( T *ptr ) const
	{
		typename MapType::right_const_iterator i = m_pool.right.find( ptr ) ;
		if ( i != m_pool.right.end() )
			return i->second ;
		else
			return Ref( ) ;
	}

private :
	typedef	boost::bimap<
		boost::bimaps::set_of<Ref>,
		boost::bimaps::set_of<T*>
	> MapType ; 
	
	typedef typename MapType::value_type MapEntry ;
	
	MapType	m_pool ;
} ;

} // end of namespace

#endif // RESOURCEPOOL_HH_
