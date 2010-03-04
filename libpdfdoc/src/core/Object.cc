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

/*!
	\file	Object.cc
	\brief	implementation the Object class
	\date	Wed Mar 5 2008
	\author	Nestal Wan
*/

#include "Object.hh"

#include "Array.hh"
#include "Dictionary.hh"
#include "String.hh"
#include "Token.hh"
#include "TokenSrc.hh"

#include "stream/Stream.hh"

#include "util/Util.hh"
#include "util/Debug.hh"

#include <boost/variant/apply_visitor.hpp>

#include <cassert>
#include <istream>
#include <iomanip>
#include <limits>
#include <map>
#include <ostream>
#include <stdexcept>

#include <iostream>

namespace pdf {

struct Bool
{
	operator bool( ) const { return value ; }
	bool value ;
	friend std::istream& operator>>( std::istream& is, Bool& b ) ;
	friend std::ostream& operator<<( std::ostream& os, const Bool& b ) ;
	friend TokenSrc& operator>>( TokenSrc& os, Bool& b ) ;
	bool operator==( const Bool& b ) const { return value == b.value ; }
} ;

/**	\brief	default constructor

	The default constructor will construct a null object.
	\post	IsNull() returns true.
*/
Object::Object( )
{
}

/*!	\brief	copy constructor
	\internal
	
	The copy constructor will deep copy the object. For example if \a obj
	is an array of dictionaries, all the individual dictionaries in the
	array will be copied. It can be slow for large objects.
	
	\param	obj		object to be copied from
*/
Object::Object( const Object& obj )
	: m_obj( obj.m_obj )
{
}

template <typename T>
Object::Object( const T& v ) : m_obj( v )
{
}

///	Construct a PDF string object.
/**	This constructor will construct a string object.
	\post	Is<std::string>() returns true
	\param	a	the content of the string 
*/
Object::Object( const char *a )
	: m_obj( std::string( a ) )
{
}

// explicit instanciation for known types
template Object::Object( const Null& a ) ;
template Object::Object( const int& a ) ;
template Object::Object( const double& a ) ;
template Object::Object( const bool& a ) ;
template Object::Object( const std::string& a ) ;
template Object::Object( const Name& a ) ;
template Object::Object( const Stream& a ) ;
template Object::Object( const Ref& a ) ;
template Object::Object( const Array& a ) ;
template Object::Object( const Dictionary& a ) ;

/**	This constructor will construct an integer object.
	
	\post	IsType<int>() == true
	\param	st	The value of the integer object.
	\throw	out_of_range	if the std::size_t input argument \c st is too
							larget to be stored in an integer.
*/
Object::Object( std::size_t st )
	: m_obj( static_cast<int>( st ) )
{
	if ( st > static_cast<std::size_t>( std::numeric_limits<int>::max( ) ) )
		throw std::out_of_range( "integer out of range" ) ;
}

Object::Object( float value )
	: m_obj( static_cast<double>( value ) )
{
}

/**	\brief	destructor
	\internal
	
	The destructor will do nothing. It is present because of the contained
	incomplete types in the member variables.
*/
Object::~Object( )
{
}

template <> Object& Object::As( )
{
    return *this ;
}

template <> const Object& Object::As( ) const
{
    return *this ;
}

bool Object::IsNumber( ) const
{
	return Type() == integer || Type() == floating ;
}

template <> bool Object::Is<Ref>( ) const
{
	return Type() == ref ;
}

template <> bool Object::Is<int>( ) const
{
	return Type() == integer ;
}

template <> bool Object::Is<double>( ) const
{
	return Type() == floating ;
}

template <> bool Object::Is<bool>( ) const
{
	return Type() == boolean ;
}

template <> bool Object::Is<std::string>( ) const
{
	return Type() == string ;
}

template <> bool Object::Is<Name>( ) const
{
	return Type() == name ;
}

template <> bool Object::Is<Stream>( ) const
{
	return Type() == stream ;
}

template <> bool Object::Is<Dictionary>( ) const
{
	return Type() == dictionary ;
}

template <> bool Object::Is<Array>( ) const
{
	return Type() == array ;
}

template <> bool Object::Is<void>( ) const
{
	return Type() == null ;
}

/// needless to say...
template <> bool Object::Is<Object>( ) const
{
	return true ;
}

template <> unsigned Object::To() const
{
	return As<int>( ) ;
}

template <> unsigned short Object::To() const
{
	return As<int>( ) ;
}

template <> short Object::To() const
{
	return As<int>( ) ;
}

template <> unsigned long Object::To() const
{
	return As<int>( ) ;
}

template <> long Object::To() const
{
	return As<int>( ) ;
}

template <> float Object::To() const
{
	return static_cast<float>( To<double>() ) ;
}

template <> double Object::To() const
{
	// the PDF specification said integers can be converted to doubles
	return Is<int>() ? As<int>() : As<double>() ;
}

bool operator==( const Object& obj1, const Object& obj2 )
{
	return obj1.m_obj == obj2.m_obj ;
}

/*!	\brief	swapping two Object
	\internal
	
	This function will swap two Object's. Unfortunately this function cannot
	guarantee that it does not throw.
	\throw	std::bad_alloc	when insufficient memory for boost::variant,
							std::string, std::map and std::vector
*/
void Object::Swap( Object& obj )
{
	m_obj.swap( obj.m_obj ) ;
}

Object& Object::operator=( const Object& obj )
{
	Object temp( obj ) ;
	Swap( temp ) ;
	return *this ;
}

/*!	\brief	get the type of the object
	\internal
	
	It returns the enum representing the underlying type of the object.
	\return	the type of the object
*/
Object::ObjType Object::Type( ) const
{
	return static_cast<ObjType>( m_obj.which( ) ) ;
}

const std::type_info& Object::TypeID( ) const
{
	return m_obj.type() ;
}

/**	\brief	Returns an persistent null object.

	This function will return a static constant Object that is default
	constructed, i.e. a null object. It is used for the return values for other
	functions such as Dictionary::find().
	
	\sa	Is<void>(), Object( )
*/
const Object& Object::NullObj()
{
	static const Object null ;
	return null ;
}

template <typename T>
bool Object::DecodeObject( TokenSrc& src, const Token& tok )
{
	src.PutBack( tok ) ;
	
	T t ;
	if ( src >> t )
	{
		m_obj = t ;
		return true ;
	}
	else
		return false ;
}

bool Object::DecodeNumberOrIndirectObj( TokenSrc& is, const Token& token )
{
	if ( DecodeObject<Ref>( is, token ) )
		return true ;
	else
	{
		is.ResetState( ) ;

		Token t ;
		if ( is >> t )
		{
			const std::string& str = t.Get() ;
			assert( t == token ) ;
			
			// can't use ?: because the types are different
			if ( str.find( '.' ) != str.npos )
				m_obj = token.As<double>( ) ;
			else
				m_obj = token.As<int>( ) ;

			return true ;
		}
		else
		{
			assert( false ) ;
			return false ;
		}
	}
}

std::istream& operator>>( std::istream& is, Object& obj )
{
	// decoded token is treated as failure. only object accepted.
	TokenSrc s( is ) ;
	return (s >> obj).Stream() ;
}

/**	\brief	Read an object from a TokenSrc.
*/
TokenSrc& operator>>( TokenSrc& src, Object& obj )
{
	static const std::string numeric = "0123456789.+-" ;
	
	typedef bool (Object::*FuncPtr)( TokenSrc&, const Token& ) ;
	
	static const std::pair<const Token, FuncPtr> table[] =
	{
		std::make_pair( Token( "<<" ),		&Object::DecodeObject<Dictionary> ),
		std::make_pair( Token( "[" ),		&Object::DecodeObject<Array> ),
		std::make_pair( Token( "(" ),		&Object::DecodeObject<String> ),
		std::make_pair( Token( "<" ),		&Object::DecodeObject<String> ),
		std::make_pair( Token( "/" ),		&Object::DecodeObject<Name> ),
		std::make_pair( Token( "true" ),	&Object::DecodeObject<Bool> ),
		std::make_pair( Token( "false" ),	&Object::DecodeObject<Bool> ),
	} ;
	typedef std::map<Token, FuncPtr> FuncMap ;
	static const FuncMap map( Begin( table ), End( table ) ) ;
	
	// decode tokens from stream
	Token t ;
	if ( src >> t )
	{
		PDF_ASSERT( !t.Get().empty( ) ) ;
		
		Token key = t ;
		if ( t.Get()[0] == '(' )
			key = Token( "(" ) ;
		else if ( t.Get()[0] == '<' && t.Get() != "<<" )
			key = Token( "<" ) ;

		FuncMap::const_iterator it = map.find( key ) ;
		
		// token that represents objects with known types
		if ( it != map.end( ) )
			(obj.*it->second)( src, t ) ;
		
		// numeric tokens. can represent number (int/double) or indirect
		// objects (reference)
		else if ( numeric.find( t.Get()[0] ) != numeric.npos )
			obj.DecodeNumberOrIndirectObj( src, t ) ;
		
		// "null" represent null object of course
		// null object is a default construct Object, so no need to set
		// explicitly, i.e. nothing to do.
		// if not null object, then it is a parse error.
		else if ( t.Get() != "null" )
		{
			src.PutBack( t ) ;
			src.SetState( std::ios::failbit ) ;
		}
	}
		
	return src ;
}

namespace {

struct Op : public boost::static_visitor<>
{
	std::ostream& m_os ;
	Op( std::ostream& os ) : m_os( os )
	{
	}
	
	template <typename T>
	void operator()( const T& t ) const
	{
		m_os << std::dec << t ;
	}
	
	void operator()( double val ) const
	{
		m_os << std::setprecision( 2 ) << std::fixed << val ;
	}
	
	void operator()( const std::string& str ) const
	{
		m_os << String(str) ;
	}
	
	void operator()( bool b ) const
	{
		m_os << std::boolalpha << b ;
	}
} ;

} // end of anonymous namespace

std::ostream& operator<<( std::ostream& os, const Object& obj )
{
	boost::apply_visitor( Op(os), obj.m_obj ) ;

	return os ;
}

std::istream& operator>>( std::istream& is, Object::Null& )
{
	Token t ;
	if ( !(is >> t) || t.Get() != "null" )
		is.setstate( is.failbit ) ;
		
	return is ;
}

std::ostream& operator<<( std::ostream& os, const Object::Null& )
{
	return os << "null" ;
}

std::istream& operator>>( std::istream& is, Bool& b )
{
	TokenSrc src( is ) ;
	return (src >> b).Stream() ;
}

TokenSrc& operator>>( TokenSrc& is, Bool& b )
{
	Token t ;
	if ( is >> t )
	{
		if ( t.Get() == "true" )
			b.value = true ;
		else if ( t.Get() == "false" )
			b.value = false ;
		else
			is.SetState( std::ios::failbit ) ;
	}
	
	return is ;
}

std::ostream& operator<<( std::ostream& os, const Bool& b )
{
	return os << (b.value ? "true" : "false") ;
}

bool operator!=( const Object& obj1, const Object& obj2 )
{
	return !operator==( obj1, obj2 ) ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Object& obj1, pdf::Object& obj2 )
	{
		obj1.Swap( obj2 ) ;
	}
}
