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
	\file	Object.hh
	\brief	definition the Object class
	\date	Wed Mar 5 2008
	\author	Nestal Wan
*/

#ifndef __PDF_OBJECT_HEADER_INCLUDED__
#define __PDF_OBJECT_HEADER_INCLUDED__

#include "Ref.hh"
#include "Name.hh"
// #include "Stream.hh"

#include "util/Exception.hh"

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include <string>
#include <iosfwd>
#include <sstream>
#include <stdexcept>

namespace pdf {

class Array ;
class Dictionary ;
class Stream ;
class Token ;
class TokenSrc ;

struct Null
{
	friend std::istream& operator>>( std::istream& is, Null& name ) ;
	friend std::ostream& operator<<( std::ostream& os, const Null& name ) ;
	bool operator==( const Null& name ) const { return true ; }
} ;

/*!	\brief	The PDF object class.
	
	This class represents the objects in PDF file. It is the basic building 
	blocks of a PDF file.
	
	There are serveral types of PDF objects: null, integer, double, boolean,
	name, string, stream, reference, array and dictionary. Each type has its
	own unique syntax such that the type of an object can be determined when
	reading them from a file.
*/
class Object
{
public :
	typedef boost::variant<
		Null, int, double, bool, std::string, Name,
		boost::recursive_wrapper<Stream>, Ref,
		boost::recursive_wrapper<Array>,
		boost::recursive_wrapper<Dictionary>
	> Variant ;

public :
	enum ObjType
	{
		null, integer, floating, boolean, string, name, stream, ref,
		array, dictionary
	} ;

public :
	Object( ) ;
	~Object( ) ;
	
	Object( const Object& obj ) ;
	Object( const char *str ) ;
	Object( std::size_t value ) ;
	Object( int value ) ;
	Object( float value ) ;
	Object( double value ) ;
	Object( bool value ) ;
	
	template <typename T>
	Object( const T& v ) ;

	void Swap( Object& obj ) ;
	Object& operator=( const Object& obj ) ;

	friend bool operator==( const Object& obj1, const Object& obj2 ) ;
	friend bool operator!=( const Object& obj1, const Object& obj2 ) ;

	friend std::istream& operator>>( std::istream& is, Object& obj ) ;
	friend std::ostream& operator<<( std::ostream& os, const Object& obj ) ;
	friend TokenSrc& operator>>( TokenSrc& src, Object& obj ) ;

	bool IsNull( ) const ;
	
	ObjType Type( ) const ;
	
	/*!	\ref	object_as
		\brief	Gets the underlying value in the object variant
	
		This function will honestly return the reference to the underlying
		object in the variant. If the underlying object
		is type T, the reference to it will be returned. Otherwise, exception
		will be thrown.
		
		\note	Not conversion will be performed as this function is required
				to return by reference not by value. The reference must refer
				to an object owned by this.
		\return	A constant reference to the underlying object of type T.
		\throw	InvalidType	The underlying object is not of type T. The
							InvalidType::what() function will describe the
							expected and actual type.
	*/
	template <typename T>
	const T& As( ) const
	{
		try
		{
			return boost::get<T>( m_obj ) ;
		}
		catch ( std::exception& e )
		{
			throw InvalidType( TypeID(), typeid(T), e ) ;
		}
	}
	
	/*!	\brief	non-constant version of As()
		\see	object_as
	*/
	template <typename T>
	T& As( )
	{
		try
		{
			return boost::get<T>( m_obj ) ;
		}
		catch ( std::exception& e )
		{
			throw InvalidType( TypeID(), typeid(T), e ) ;
		}
	}
	
	template <typename T>
	operator T() const
	{
		return As<T>( ) ;
	}
	
	template <typename F>
	void Visit( F func ) const
	{
		boost::apply_visitor( func, m_obj ) ;
	}

	template <typename F>
	void Visit( F func )
	{
		boost::apply_visitor( func, m_obj ) ;
	}

	const std::type_info& TypeID( ) const ;

private :
	// private function called only by non-template members,
	// so it can be defined in the cpp file.
	template <typename T>
	bool DecodeObject( TokenSrc& is, const Token& t ) ;

	bool DecodeNumberOrIndirectObj( TokenSrc& is, const Token& token ) ;

private :
	Variant	m_obj ;
} ;

template <> Object::operator long() const ;
template <> Object::operator double() const ;
template <> Object::operator float() const ;


} // end of namespace

#endif
