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

/**	\file	ArrayReader.hh
    \brief	definition the ArrayReader class
    \date	Feb 7, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_ARRAYREADER_HH_EADER_INCLUDED__
#define __PDF_ARRAYREADER_HH_EADER_INCLUDED__

#include "ElementPool.hh"
#include "File.hh"

#include "core/Array.hh"

namespace pdf {

class File ;

///	brief description
/**	\internal
	The ArrayReader class represents
*/
class ArrayReader
{
public :
	ArrayReader( ) ;
	ArrayReader( Array& array, File *file ) ;
	ArrayReader( Object& obj, File *file ) ;

	template <typename T>
	bool Detach( std::size_t idx, T& result ) ;

	template <typename T>
	T At( std::size_t idx ) const
	{
		if ( idx < m_array.size() )
		{
			const Object& obj = m_array[idx] ;
			
			// if it is what you want, then return it
			if ( obj.Is<T>() )
				return obj.To<T>() ;
			
			// if it is a Ref instead, read the file for it and try to convert
			// it to T.
			// note that if it is a Ref but T is really a Ref, that means the
			// caller said she want a Ref explicit, we don't de-reference it.
			else if ( obj.Is<Ref>() )
				return m_file->ReadObj( obj.As<Ref>() ).To<T>() ;
		}
		
		// nothing we can do
		return T() ;	
	}

	template <typename Func>
	typename Func::result_type Create( std::size_t i, Func func )
	{
		typename Func::result_type dummy = 0 ;
		return Create( i, func, dummy ) ;
	}

	template <typename Element, typename Func>
	Element* Create( std::size_t i, Func func, Element *original )
	{
		ElementPool *pool = m_file ? m_file->Pool() : 0 ;
		Element *result = 0 ;
		
		// it's good if it's a reference to something already in the pool
		if ( i < m_array.size() && m_array[i].Is<Ref>() && pool != 0 )
			result = pool->Acquire<Element>( m_array[i] ) ;

		// otherwise we need to create it and maybe add it in the pool
		if ( result == 0 && i < m_array.size() )
		{
			typename Element::BaseType temp ;
			bool is_ref = m_array[i].Is<Ref>() ;
			Detach( i, temp ) ;
			result = func( temp ) ;
			
			if ( is_ref && pool != 0 && result != 0 )
				pool->Add( m_array[i], result ) ;
		}
	
		if ( result != 0 && original != 0 )
			original->Release() ;
		
		return result ;
	}

	// pointer-like operations for dictionary
	Array* operator->() ;
	const Array* operator->() const ;

	Array& operator*() ;
	const Array& operator*() const ;

	// wrapper for operator[]
	Object& operator[]( std::size_t idx ) ;
	const Object& operator[]( std::size_t idx ) const ;

	File* GetFile( ) const ;
	void SetFile( File *file ) ;

private :
	Array	m_array ;
	File	*m_file ;
} ;

} // end of namespace

#endif // ARRAYREADER_HH_
