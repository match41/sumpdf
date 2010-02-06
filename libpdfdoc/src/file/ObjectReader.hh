/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/*!
	\file	ObjectReader.hh
	\brief	definition the ObjectReader class
	\date	Dec 11, 2009
	\author	nestal
*/


#ifndef __PDF_OBJECT_READER_HEADER_INCLUDED__
#define __PDF_OBJECT_READER_HEADER_INCLUDED__

#include "File.hh"
#include "ResourcePool.hh"
#include "core/Object.hh"
#include "core/Dictionary.hh"

#include <boost/bind.hpp>

namespace pdf {

template <typename ObjType>
bool DetachTo( File *file, Dictionary& dict, const Name& name, ObjType& result )
{
	Dictionary::iterator i = dict.find( name ) ;
	if ( i != dict.end( ) )
	{
		result = ( i->second.Type( ) == Object::ref ) ?
			file->ReadObj( i->second ).To<ObjType>() :
			i->second.To<ObjType>() ;

		dict.erase( i ) ;
		return true ;
	}
	return false ;
}

template <typename T>
struct Detacher
{
	bool operator()( File *file, Dictionary& dict, const Name& name, T& result )
	{
		Dictionary::iterator i = dict.find( name ) ;
		if ( i != dict.end( ) )
		{
			if ( i->second.Type( ) == Object::ref )
				result = file->ReadObj( i->second ).As<T>() ;
			else
				std::swap( i->second.As<T>(), result ) ;

			dict.erase( i ) ;
			return true ;
		}
		return false ;
	}
} ;

template <>
struct Detacher<int>
{
	bool operator()( File *file, Dictionary& dict, const Name& name, int& t )
	{
		return DetachTo( file, dict, name, t ) ;
	}
} ;

template <>
struct Detacher<double>
{
	bool operator()( File *file, Dictionary& dict, const Name& name, double& t )
	{
		return DetachTo( file, dict, name, t ) ;
	}
} ;

template <typename ObjType>
bool Detach( File *file, Dictionary& dict, const Name& name, ObjType& result )
{
	return Detacher<ObjType>()( file, dict, name, result ) ;
}

template <typename ObjType>
bool DeRef( File *file, const Dictionary& dict, const Name& name,
            ObjType& result )
{
	Dictionary::const_iterator i = dict.find( name ) ;
	if ( i != dict.end( ) )
	{
		if ( i->second.Type( ) == Object::ref )
			result = file->ReadObj( i->second ) ;
		else
			result = i->second.As<ObjType>() ;

		return true ;
	}
	return false ;
}

template <typename ObjType>
ObjType DeRefObj( File *file, const Object& obj )
{
	if ( obj.Is<Ref>( ) )
	{
		return file->ReadObj( obj.As<Ref>() ) ;
	}
	else
		return obj.As<ObjType>( )  ;
}

template <typename ObjType>
ObjType& DeRefObj( File *file, Object& src, ObjType& dest )
{
	if ( src.Is<Ref>( ) )
	{
		ObjType obj = file->ReadObj( src.As<Ref>() ) ;
		std::swap( dest, obj ) ;
	}
	else if ( !src.Is<void>() )
		std::swap( dest, src.As<ObjType>( ) ) ;
	
	return dest ;
}

} // end of namespace

#endif // OBJECTREADER_HH_
