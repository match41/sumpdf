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
#include "core/Object.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"

#include <utility>
#include <vector>

namespace pdf {

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
	bool Detach( const Name& name, ObjType& result )
	{
		Dictionary::iterator i = m_dict.find( name ) ;
		if ( i != m_dict.end() )
		{
			At( i, result ) ;
			m_dict.erase( i ) ;
			return true ;
		}
		else
			return false ;
	}

	template <typename T>
	bool At( Dictionary::iterator i, T& result ) ;

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
				return m_file->ReadObj( obj.As<Ref>() ).To<T>() ;
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

	// wrapper for operator[]
	Object& operator[]( const Name& name ) ;
	const Object& operator[]( const Name& name ) const ;

	File* GetFile( ) const ;
	void SetFile( File *file ) ;

private :
	Dictionary	m_dict ;
	File		*m_file ;
} ;

} // end of namespace

#endif // DICTREADER_HH_
