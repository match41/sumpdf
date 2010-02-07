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

#include "core/Array.hh"

#include "File.hh"

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
			// if it is what you want, then return it
			if ( m_array[idx].Is<T>() )
				return m_array[idx].To<T>() ;
			
			// if it is a Ref instead, read the file for it and try to convert
			// it to T.
			// note that if it is a Ref but T is really a Ref, that means the
			// caller said she want a Ref explicit, we don't de-reference it.
			else if ( m_array[idx].Is<Ref>() )
				return m_file->ReadObj( m_array[idx].As<Ref>() ).To<T>() ;
		}
		
		// nothing we can do
		return T() ;	
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
