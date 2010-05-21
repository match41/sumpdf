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

/**	\file	ResourceSet.hh
    \brief	definition the ResourceSet class
    \date	May 21, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_RESOURCESET_HH_EADER_INCLUDED__
#define __PDF_RESOURCESET_HH_EADER_INCLUDED__

#include "core/Dictionary.hh"
#include "core/Name.hh"

#include "file/ElementFactory.hh"

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

namespace pdf {

class DictReader ;

///	brief description
/**	\internal
	The ResourceSet class represents
*/
template <typename T>
class ResourceSet
{
public :
	explicit ResourceSet( const std::string& prefix ) ;
	~ResourceSet( ) ;

	void Clear( ) ;
	
	Name Add( T *t ) ;
	Name Find( const T *t ) const ;
	T* Find( const Name& name ) const ;

private :
	typedef	boost::bimap<
		boost::bimaps::set_of<Name>,
		boost::bimaps::set_of<T*>
	> Map ; 

public :
	typedef typename Map::left_const_iterator	iterator ;
	
public :
	template <typename Func>
	void MassProduce( DictReader& dict, Func func )
	{
		ElementFactory<> factory( dict ) ;
		factory.MassProduce<T>( func, std::inserter(
			m_map.left, m_map.left.end() ) ) ;
	}

	iterator begin( ) const ;
	iterator end( ) const ;

private :
	Map	m_map ;
	
	const std::string	m_prefix ;
} ;

} // end of namespace

#endif // RESOURCESET_HH_
