/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
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
 ***************************************************************************/

/*!
	\file	ElementList.hh
	\brief	definition the IElementVisitor class
	\date	Sat Apr 19 2008
	\author	Nestal Wan
*/

#ifndef __PDF_ELEMENT_LIST_HEADER_INCLUDED__
#define __PDF_ELEMENT_LIST_HEADER_INCLUDED__

#include <vector>

// #include <boost/iterator/indirect_iterator.hpp>

namespace pdf {

class IElement ;

/*!	\brief	a list of element
	
	this class represents a list of elements. this class does not own the
	contained elements. it won't delete the element pointers in its
	destructor.
*/
class ElementList
{
private :
	typedef std::vector<IElement*> Vec ;

public :
	typedef Vec::iterator			iterator ;
	typedef Vec::const_iterator		const_iterator ;	
	typedef Vec::reference			reference ;
	typedef Vec::const_reference	const_reference ;

public :
	explicit ElementList( std::size_t size = 0 ) ;

	template <typename InputIt>
	ElementList( InputIt first, InputIt last )
		: m_vec( first, last )
	{
	}

	std::size_t size( ) const ;
	bool empty( ) const ;
	
	iterator begin( ) ;
	iterator end( ) ;
	
	const_iterator begin( ) const ;
	const_iterator end( ) const ;

	void push_back( IElement *element ) ;
	
	template <typename InputIt>
	void assign( InputIt first, InputIt last )
	{
		m_vec.assign( first, last ) ;
	}

private :
	Vec	m_vec ;
} ;

} // end of namespace

#endif
