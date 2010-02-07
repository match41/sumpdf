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

#include <boost/bind.hpp>

namespace pdf {

///	brief description
/**	\internal
	The DictReader class represents
*/
class DictReader
{
public :
	DictReader( Dictionary& dict, File *file ) ;
	
	template <typename ObjType>
	bool Detach( const Name& name, ObjType& result ) ;

	template <typename ObjType>
	bool DeRef( const Name& name, ObjType& result )
	{
		Dictionary::const_iterator i = m_dict.find( name ) ;
		if ( i != m_dict.end( ) )
		{
			result = i->second.Is<Ref>()
				? m_file->ReadObj( i->second )
				: i->second.As<ObjType>() ;

			return true ;
		}
		return false ;
	}

	template <typename ObjType>
	ObjType DeRefObj( const Object& obj )
	{
		if ( obj.Is<Ref>( ) )
		{
			return m_file->ReadObj( obj.As<Ref>() ) ;
		}
		else
			return obj.As<ObjType>( )  ;
	}

	template <typename ObjType>
	ObjType& DeRefObj( Object& src, ObjType& dest )
	{
		if ( src.Is<Ref>( ) )
		{
			ObjType obj = m_file->ReadObj( src.As<Ref>() ) ;
			std::swap( dest, obj ) ;
		}
		else if ( !src.Is<void>() )
			std::swap( dest, src.As<ObjType>( ) ) ;
		
		return dest ;
	}

	// pointer-like operations for dictionary
	Dictionary* operator->() ;
	const Dictionary* operator->() const ;

	Dictionary& operator*() ;
	const Dictionary& operator*() const ;

	// wrapper for operator[]
	Object& operator[]( const Name& name ) ;
	const Object& operator[]( const Name& name ) const ;

private :
	Dictionary	m_dict ;
	File		*m_file ;
} ;

} // end of namespace

#endif // DICTREADER_HH_
