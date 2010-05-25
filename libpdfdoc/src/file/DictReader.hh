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

/**	\file	DictReader.hh
    \brief	definition the DictReader class
    \date	Feb 7, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_DICTREADER_HH_EADER_INCLUDED__
#define __PDF_DICTREADER_HH_EADER_INCLUDED__

#include "File.hh"
#include "ElementPool.hh"

#include "core/Object.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"

//#include "util/Util.hh"

#include <utility>
#include <vector>

namespace pdf {

template <typename T> struct NewPtr ;

///	brief description
/**	\internal
	The DictReader class represents
*/
class DictReader
{
public :
	DictReader( ) ;
	DictReader( Dictionary& dict, File *file ) ;
	DictReader( Object& obj, File *file ) ;
	
	void Swap( DictReader& dict ) ;
	
	template <typename ObjType>
	bool Detach( const Name& name, ObjType& result ) ;

	template <typename T>
	bool SwapAt( Dictionary::iterator i, T& result ) ;

	template <typename T>
	bool SwapAt( Dictionary::iterator i, std::vector<T>& result ) ;
	
	template <typename Func>
	typename Func::result_type Create( const Name& name, Func func )
	{
		typename Func::result_type dummy = 0 ;
		return Create( name, func, dummy ) ;
	}
	
	template <typename Func>
	typename Func::result_type Create( Dictionary::iterator i, Func func )
	{
		typename Func::result_type dummy = 0 ;
		return Create( i, func, dummy ) ;
	}
	
	template <typename Element, typename Func>
	Element* Create( const Name& name, Func func, Element *original )
	{
		Dictionary::iterator i = m_dict.find(name) ;
		Element *e = Create( i, func, original ) ;
		if ( e != 0 && i != m_dict.end() )
			m_dict.erase(i) ;
		return e ;
	}
	
	template <typename Element, typename Func>
	Element* Create( Dictionary::iterator i, Func func, Element *original )
	{
		ElementPool *pool = m_file ? m_file->Pool() : 0 ;
		Element *result = 0 ;
		
		// it's good if it's a reference to something already in the pool
		if ( i != m_dict.end() && i->second.Is<Ref>() && pool != 0 &&
		     pool->Acquire( i->second, result ) )
		{
		}

		// otherwise we need to create it and maybe add it in the pool
		else if ( i != m_dict.end() )
		{
			typename Element::BaseType temp ;
			bool is_ref = SwapAt( i, temp ) ;
			result = func( temp ) ;
			
			if ( is_ref && pool != 0 && result != 0 )
				pool->Add( i->second, result ) ;
		}
	
		if ( result != 0 && original != 0 )
			original->Release() ;
		
		return result ;
	}
	
	template <typename T, typename Iterator>
	T At( Iterator i ) const
	{
		if ( i != m_dict.end() )
		{
			const Object& obj = i->second ;
		
			// if it is what you want, then return it
			if ( obj.Is<T>() )
				return obj.To<T>() ;
			
			// if it is a Ref instead, read the file for it and try to convert
			// it to T.
			// note that if it is a Ref but T is really a Ref, that means the
			// caller said she want a Ref explicit, we don't de-reference it.
			else if ( obj.Is<Ref>() )
			{
				T temp ;
				m_file->ReadType( obj, temp ) ;
				return temp ;
			}
		}
		
		// nothing we can do
		return T() ;	
	}

	template <typename T>
	T At( const Name& name ) const
	{
		return At<T>( m_dict.find( name ) ) ;
	}

	template <typename T>
	T At( const char *name ) const
	{
		return At<T>( m_dict.find( Name(name) ) ) ;
	}

	// pointer-like operations for dictionary
	Dictionary* operator->() ;
	const Dictionary* operator->() const ;

	Dictionary& operator*() ;
	const Dictionary& operator*() const ;

	Dictionary& Get() ;
	const Dictionary& Get() const ;

	// wrapper for operator[]
	const Object& operator[]( const Name& name ) const ;

	File* GetFile( ) const ;
	void SetFile( File *file ) ;

	template <typename Element, typename ObjType>
	bool Acquire( const Name& name, Element* &element, ObjType& object ) ;

private :
	Dictionary	m_dict ;
	File		*m_file ;
} ;

} // end of namespace

#endif // DICTREADER_HH_
