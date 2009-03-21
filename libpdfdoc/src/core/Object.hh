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

/*!	\brief	The PDF object class.
	\internal
	
	This class represents the objects in PDF file. It is the basic building 
	block of a PDF file.
	
	There are serveral types of PDF objects: null, integer, double, boolean,
	name, string, stream, reference, array and dictionary. Each type has its
	own unique syntax such that the type of an object can be determined when
	reading them from a file.
	
	PDF Object can be recursive. Object can be arrays with can contain other
	objects as well.
	
	This class is implemented using boost::variant, which is a generic
	stack-base discriminated union container. It can contain different types
	of data in a single object, but only one type of data is active. The
	boost::variant template we use can containthe PDF object types: null,
	integer, double, boolean, name, string, stream, reference, array and
	dictionary.
	
	The As() functions provides a type-safe way to obtain the underlying
	value in an Object. Callers should supply the template parameter \a T
	when calling As(). For example:

\code
Object a( "hello" ) ;
std::string& astr = a.As<std::string>() ;
\endcode
*/
class Object
{
private :
	/*!	\brief	dummy null object
		\internal
		
		This struct is a dummy placeholder for null value. It has no value
		so all Null objects are equal.
	*/
	struct Null
	{
		/// \internal all null objects are by definition equal
		bool operator==( const Null& name ) const { return true ; }
	} ;
	
	friend std::istream& operator>>( std::istream& is, Null& ) ;
	friend std::ostream& operator<<( std::ostream& os, const Null& ) ;

	/*!	\brief	boost variant typedef
		\internal
		
		This typedef is a shortcut to the long template name. A must have.
	*/
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
	/*!	\brief	default constructor
		\internal
	
		The default constructor will construct a null object.
		\post	IsNull() returns true.
	*/
	Object( ) ;
	
	/*!	\brief	destructor
		\internal
		
		The destructor will do nothing. It is present because of the contained
		incomplete types in the member variables.
	*/
	~Object( ) ;
	
	/*!	\brief	copy constructor
		\internal
		
		The copy constructor will deep copy the object. For example if \a obj
		is an array of dictionaries, all the individual dictionaries in the
		array will be copied. It can be slow for large objects.
		
		\param	obj		object to be copied from
	*/
	Object( const Object& obj ) ;
	
	/*!	\internal This constructor will construct a string object.
		\post	Type() == string
		\param	str	the content of the string 
	*/
	Object( const char *str ) ;
	Object( std::size_t value ) ;
	Object( float value ) ;
	
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
	
	/*!	\brief	Gets the underlying value in the object variant
		\internal
	
		This function will honestly return the reference to the underlying
		object in the variant. If the underlying object
		is type \a T, the reference to it will be returned. Otherwise,
		exception will be thrown.
		
		\note	Not conversion will be performed as this function is required
				to return by reference not by value. The reference must refer
				to an object owned by this.
		
		\warning	The object referred by the returned reference will be
					destroyed when the type of the Object changes.
		
		\return	A constant reference to the underlying object of type \a T.
		\throw	BadType	The underlying object is not of type \a T. The
						BadType::what() function will describe the
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
			throw BadType( TypeID(), typeid(T), e ) ;
		}
	}
	
	/*!	\brief	non-constant version of As()
		\internal
		\sa	As()
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
			throw BadType( TypeID(), typeid(T), e ) ;
		}
	}
	
	/*!	\brief	conversion operator to arbitrary types
		\internal
		
		This function is similar to As(), except it returns the result
		by value, not by reference. The advantage is that automatic conversion
		is possible even if \a T is not one of the supported types (e.g. long).
		The disadvantage is that \a T must be copied by its copy constructor.
		For large objects like Array and Dictionary, there may be performance
		problems.
		
		\throw	BadType	The underlying object is not of type \a T. The
						BadType::what() function will describe the
						expected and actual type.
		\sa As()
	*/
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

} // end of namespace

namespace std
{
	void swap( pdf::Object& obj1, pdf::Object& obj2 ) ;
}

#endif