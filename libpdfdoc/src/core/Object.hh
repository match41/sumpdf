/***************************************************************************
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
#include "Token.hh"
#include "util/Exception.hh"

#include <boost/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

#include <string>
#include <iosfwd>
#include <new>
#include <vector>

namespace pdf {

class Array ;
class Dictionary ;
class Rect ;
class Stream ;
class Token ;

/*!	\brief	The PDF object class.
	\internal
	
	This class represents the objects in PDF file. It is the basic building 
	block of a PDF file.
	
	There are serveral types of PDF objects:
	- \link Object::Null Null \endlink
	- integer
	- double
	- boolean
	- Name
	- String
	- Stream
	- \link Ref Reference \endlink
	- Array
	- Dictionary.
	
	Each type has its
	own unique syntax such that the type of an object can be determined when
	reading them from a file.
	
	PDF Object can be recursive. Object can be arrays with can contain other
	objects as well.
	
	This class is implemented using boost::variant, which is a generic
	stack-base discriminated union container. It can contain different types
	of data in a single object, but only one type of data is active.
	
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
		bool operator!=( const Null& name ) const { return false ; }
		bool operator<( const Null& name ) const { return false ; }
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
		boost::recursive_wrapper<Dictionary>,
		Token
	> Variant ;

public :
	enum ObjType
	{
		null, integer, floating, boolean, string, name, stream, ref,
		array, dictionary, token
	} ;

public :
	Object( ) ;
	~Object( ) ;
	Object( const Object& obj ) ;
	Object( const char *str ) ;
	Object( std::size_t value ) ;
	Object( unsigned short value ) ;
	Object( float value ) ;
	Object( const Rect& rect ) ;
	
	template <typename T>
	Object( const T& v ) ;

	template <typename T>
	Object( const std::vector<T>& v ) ;

	void Swap( Object& obj ) ;
	Object& operator=( const Object& obj ) ;

	/**	\brief	Swapping the underlying value.
	
		This function will swap the underlying value with the argument.
		\tparam	T	The type of the expecting underlying value. If the
					underlying value of the Object is not of type \a T,
					a BadType exception will be thrown.
		\param	t	The value to be swapped.
		\throw	BadType	Throw if \a T is not the type of the underlying
						value.
	*/
	template <typename T>
	void Swap( T& t )
	{
		std::swap( As<T>(), t ) ;
	}

	friend bool operator==( const Object& obj1, const Object& obj2 ) ;
	friend bool operator!=( const Object& obj1, const Object& obj2 ) ;
	friend bool operator<( const Object& obj1, const Object& obj2 ) ;

	friend std::istream& operator>>( std::istream& is, Object& obj ) ;
	friend std::ostream& operator<<( std::ostream& os, const Object& obj ) ;
	
	static const Object& NullObj() ;
	
	ObjType Type( ) const ;
	bool IsNumber( ) const ;
	
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
		
		\tparam	T	the target type of the value.
		\return	A constant reference to the underlying object of type \a T.
		\throw	BadType	The underlying object is not of type \a T. The
						BadType::what() function will describe the
						expected and actual type.
	*/
	template <typename T>
	const T& As( ) const ;
	
	/*!	\brief	non-constant version of As()
		\internal
		\sa	As()
	*/
	template <typename T>
	T& As( ) ;
	
	///	Conversion operator to arbitrary types
	/**	This function calls To(). It is useful to automatically converts
		the Object to the underlying types.
		
		\tparam	T		The target type to be converted to.
		\throw	BadType	The underlying object is not of type \a T. The
						BadType::what() function will describe the
						expected and actual type.
		\sa As(), To()
	*/
	template <typename T>
	operator T() const ;

	template <typename T>
	T To( std::nothrow_t ) const ;

	///	Conversion function to arbitrary types
	/**	This function is similar to As(), except it returns the result
		by value, not by reference. The advantage is that automatic conversion
		is possible even if \a T is not one of the supported types (e.g. long).
		The disadvantage is that \a T must be copied by its copy constructor.
		For large objects like Array and Dictionary, there may be performance
		problems.
		
		\tparam	T		The target type to be converted to.
		\throw	BadType	The underlying object is not of type \a T. The
						BadType::what() function will describe the
						expected and actual type.
		\sa As()
	*/
	template <typename T>
	T To( ) const ;
	
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

	/**	\brief	Check the type of the Object.
    
		Check if the type of the object is the same as the template argument.
		This function is preferred over Type() because you don't have to
		memorize the members of the ObjType enum.
		\tparam	T		the type to be check against
		\return	\c true if the contained object is of type \a T, otherwise
				\c false.
		\sa	Type(), As(), To()
    */
    template <typename T>
    bool Is( ) const ;

	const std::type_info& TypeID( ) const ;

	std::string DumpString() const ;

private :
	// private function called only by non-template members,
	// so it can be defined in the cpp file.
	template <typename T>
	bool DecodeObject( std::istream& is ) ;

	bool DecodeNumberOrIndirectObj( std::istream& is ) ;

	template <typename T>
	std::vector<T> ToVec( ) const ;

private :
	Variant	m_obj ;
} ;

} // end of namespace

#ifdef __GNUC__
// extern template is a gcc extension that prevents the compiler from
// automatically instantiating a template to save time. the template will be
// explicitly instantiated in the .cc file instead.
extern template class boost::variant<
	pdf::Object::Null, int, double, bool, std::string, pdf::Name,
	boost::recursive_wrapper<pdf::Stream>, pdf::Ref,
	boost::recursive_wrapper<pdf::Array>,
	boost::recursive_wrapper<pdf::Dictionary>
	> ;
#endif

namespace std
{
	void swap( pdf::Object& obj1, pdf::Object& obj2 ) ;
}

#endif
