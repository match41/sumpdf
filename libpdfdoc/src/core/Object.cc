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

#include "stream/Stream.hh"

#include "util/Debug.hh"
#include "util/Rect.hh"
#include "util/CArray.hh"

#include <boost/variant/apply_visitor.hpp>

#include <cassert>
#include <istream>
#include <iomanip>
#include <limits>
#include <map>
#include <ostream>
#include <stdexcept>

#include <iostream>
#include <sstream>

#ifdef __GNUC__
template class boost::variant<
	pdf::Object::Null, int, double, bool, std::string, pdf::Name,
	boost::recursive_wrapper<pdf::Stream>, pdf::Ref,
	boost::recursive_wrapper<pdf::Array>,
	boost::recursive_wrapper<pdf::Dictionary>
	> ;
#endif

namespace pdf {

template <> unsigned short	Object::To( ) const ;
template <> short			Object::To( ) const ;
template <> unsigned		Object::To( ) const ;
template <> long			Object::To( ) const ;
template <> unsigned long	Object::To( ) const ;
template <> float			Object::To( ) const ;
template <> double			Object::To( ) const ;
template <> Rect 			Object::To( ) const ;
template <> Object& Object::As( ) ;
template <> const Object& Object::As( ) const ;

template <> std::vector<int> Object::To() const ;
template <> std::vector<double> Object::To() const ;
template <> std::vector<short> Object::To() const ;
template <> std::vector<unsigned> Object::To() const ;
template <> std::vector<long> Object::To() const ;
template <> std::vector<unsigned long> Object::To() const ;
template <> std::vector<float> Object::To() const ;
template <> std::vector<Rect> Object::To() const ;
template <> std::vector<Object> Object::To() const ;

struct Bool
{
	operator bool( ) const { return value ; }
	bool value ;
	friend std::istream& operator>>( std::istream& is, Bool& b ) ;
	friend std::ostream& operator<<( std::ostream& os, const Bool& b ) ;
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

Object::Object( unsigned short value )
	: m_obj( static_cast<int>( value ) )
{
}

Object::Object( float value )
	: m_obj( static_cast<double>( value ) )
{
}

Object::Object( const Rect& rect )
	: m_obj( Array( rect.begin(), rect.end() ) )
{
}

template <typename T>
Object::Object( const std::vector<T>& v )
	: m_obj( Array( v.begin(), v.end() ) )
{
}

template Object::Object( const std::vector<int>& ) ;
template Object::Object( const std::vector<double>& ) ;
template Object::Object( const std::vector<std::string>& ) ;
template Object::Object( const std::vector<Name>& ) ;
template Object::Object( const std::vector<Stream>& ) ;
template Object::Object( const std::vector<Ref>& ) ;
template Object::Object( const std::vector<Array>& ) ;
template Object::Object( const std::vector<Dictionary>& ) ;
template Object::Object( const std::vector<Object>& ) ;
template Object::Object( const std::vector<float>& ) ;
template Object::Object( const std::vector<std::size_t>& ) ;
template Object::Object( const std::vector<const char*>& ) ;
template Object::Object( const std::vector<Rect>& ) ;

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

template <> bool Object::Is<Token>( ) const
{
	return Type() == token ;
}

/// needless to say...
template <> bool Object::Is<Object>( ) const
{
	return true ;
}

template <typename T>
const T& Object::As( ) const
{
	try
	{
		return boost::get<T>( m_obj ) ;
	}
	catch ( std::exception& e )
	{
		throw BadType()
			<< expt::SourceType( TypeID().name() )
			<< expt::DestType( TypeID().name() )
			<< expt::ErrMsg( e.what() )
			<< expt::Token( DumpString() ) ;
	}
}

/*!	\brief	non-constant version of As()
	\internal
	\sa	As()
*/
template <typename T>
T& Object::As( )
{
	try
	{
		return boost::get<T>( m_obj ) ;
	}
	catch ( std::exception& e )
	{
		throw BadType()
			<< expt::SourceType( TypeID().name() )
			<< expt::DestType( typeid(T).name() )
			<< expt::ErrMsg( e.what() )
			<< expt::Token( DumpString() ) ;
	}
}

template int& Object::As() ;
template const int& Object::As() const ;
template double& Object::As() ;
template const double& Object::As() const ;
template Ref& Object::As() ;
template const Ref& Object::As() const ;
template Name& Object::As() ;
template const Name& Object::As() const ;
template std::string& Object::As() ;
template const std::string& Object::As() const ;
template bool& Object::As() ;
template const bool& Object::As() const ;
template Stream& Object::As() ;
template const Stream& Object::As() const ;
template Array& Object::As() ;
template const Array& Object::As() const ;
template Dictionary& Object::As() ;
template const Dictionary& Object::As() const ;
template Token& Object::As() ;
template const Token& Object::As() const ;

template <typename T>
Object::operator T() const
{
	return To<T>( ) ;
}

template Object::operator int() const ;
template Object::operator bool() const ;
template Object::operator double() const ;
template Object::operator Ref() const ;
template Object::operator Name() const ;
template Object::operator std::string() const ;
template Object::operator Stream() const ;
template Object::operator Array() const ;
template Object::operator Dictionary() const ;
template Object::operator Token() const ;

template Object::operator unsigned() const ;
template Object::operator unsigned short() const ;
template Object::operator short() const ;
template Object::operator unsigned long() const ;
template Object::operator long() const ;
template Object::operator float() const ;
template Object::operator Rect() const ;

template <typename T>
T Object::To( std::nothrow_t ) const
{
	try
	{
		return To<T>() ;
	}
	catch ( BadType& )
	{
		return T() ;
	}
}

template int Object::To( std::nothrow_t ) const ;
template bool Object::To( std::nothrow_t ) const ;
template double Object::To( std::nothrow_t ) const ;
template Ref Object::To( std::nothrow_t ) const ;
template Name Object::To( std::nothrow_t ) const ;
template std::string Object::To( std::nothrow_t ) const ;
template Stream Object::To( std::nothrow_t ) const ;
template Array Object::To( std::nothrow_t ) const ;
template Dictionary Object::To( std::nothrow_t ) const ;
template Token Object::To( std::nothrow_t ) const ;

template unsigned Object::To( std::nothrow_t ) const ;
template unsigned short Object::To( std::nothrow_t ) const ;
template unsigned long Object::To( std::nothrow_t ) const ;
template short Object::To( std::nothrow_t ) const ;
template long Object::To( std::nothrow_t ) const ;
template float Object::To( std::nothrow_t ) const ;
template Rect Object::To( std::nothrow_t ) const ;

template <typename T>
T Object::To( ) const
{
	return As<T>() ;
}

template bool Object::To( ) const ;
template Ref Object::To( ) const ;
template Name Object::To( ) const ;
template std::string Object::To( ) const ;
template Stream Object::To( ) const ;
template Array Object::To( ) const ;
template Dictionary Object::To( ) const ;
template Token Object::To( ) const ;

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

template <> int Object::To() const
{
	// the PDF specification said integers can be converted to doubles
	return Is<int>() ? As<int>() : static_cast<int>(As<double>()) ;
}

template <typename T>
std::vector<T> Object::ToVec( ) const
{
	const Array& a = As<Array>() ;
	return std::vector<T>( a.begin(), a.end() ) ;
}

template <> std::vector<int> Object::To() const
{
	return ToVec<int>() ;
}

template <> std::vector<double> Object::To() const
{
	return ToVec<double>() ;
}

template <> std::vector<unsigned> Object::To() const
{
	return ToVec<unsigned>() ;
}

template <> std::vector<short> Object::To() const
{
	return ToVec<short>() ;
}

template <> std::vector<unsigned short> Object::To() const
{
	return ToVec<unsigned short>() ;
}

template <> std::vector<long> Object::To() const
{
	return ToVec<long>() ;
}

template <> std::vector<unsigned long> Object::To() const
{
	return ToVec<unsigned long>() ;
}

template <> std::vector<float> Object::To() const
{
	return ToVec<float>() ;
}

template <> std::vector<Rect> Object::To() const
{
	return ToVec<Rect>() ;
}

template <> Rect Object::To() const
{
	const Array& a = As<Array>() ;
	return Rect( a.begin(), a.end() ) ;
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
bool Object::DecodeObject( std::istream& src )
{
	T t ;
	if ( src >> t )
	{
		m_obj = t ;
		return true ;
	}
	else
		return false ;
}

bool Object::DecodeNumberOrIndirectObj( std::istream& is )
{
	std::streamsize pos = is.tellg() ;

	// special handling for stream that cannot be rewinded
	if ( pos != -1 && DecodeObject<Ref>( is ) )
	{
		return true ;
	}
	else
	{
		if ( pos != -1 )
		{
			is.clear( ) ;
			is.seekg( pos ) ;
		}
		
		Token t ;
		if ( is >> t )
		{
			const std::string& str = t.Get() ;
			
			// can't use ?: because the types are different
			if ( str.find( '.' ) != str.npos )
				m_obj = t.As<double>( ) ;
			else
				m_obj = t.As<int>( ) ;

			return true ;
		}
		else
		{
			return false ;
		}
	}
}

std::istream& operator>>( std::istream& src, Object& obj )
{
	typedef bool (Object::*FuncPtr)( std::istream& ) ;
	
	static const std::pair<const Token, FuncPtr> table[] =
	{
		std::make_pair( Token( "<<" ),		&Object::DecodeObject<Dictionary> ),
		std::make_pair( Token( "[" ),		&Object::DecodeObject<Array> ),
		std::make_pair( Token( "(" ),		&Object::DecodeObject<String> ),
		std::make_pair( Token( "<" ),		&Object::DecodeObject<String> ),
		std::make_pair( Token( "/" ),		&Object::DecodeObject<Name> ),
	} ;
	typedef std::map<Token, FuncPtr> FuncMap ;
	static const FuncMap map( Begin( table ), End( table ) ) ;
	
	// decode tokens from stream
	Token t ;
	if ( Token::PeekPrefix( src, t ) )
	{
		PDF_ASSERT( !t.Get().empty( ) ) ;
		
		FuncMap::const_iterator it = map.find( t ) ;
		
		// token that represents objects with known types
		if ( it != map.end( ) )
			(obj.*it->second)( src ) ;
		
		// numeric tokens. can represent number (int/double) or indirect
		// objects (reference)
		else if ( t.IsNumber() )
			obj.DecodeNumberOrIndirectObj( src ) ;
		
		else
		{
			src >> t ;
			if ( t.Get() == "true" )
			{
				Bool b = { true } ;
				obj.m_obj = b ;
			}
			else if ( t.Get() == "false" )
			{
				Bool b = { false } ;
				obj.m_obj = b ;
			}
			// "null" represent null object of course
			else if ( t.Get() == "null" )
				obj.m_obj = Object::Null() ;
			
			// Token
			else
				obj.m_obj = t ;
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
	Token t ;
	if ( is >> t )
	{
		if ( t.Get() == "true" )
			b.value = true ;
		else if ( t.Get() == "false" )
			b.value = false ;
		else
			is.setstate( std::ios::failbit ) ;
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

bool operator<( const Object& obj1, const Object& obj2 )
{
	return obj1.m_obj < obj2.m_obj ;
}

std::string Object::DumpString() const
{
	std::ostringstream oss ;
	oss << *this ;
	return oss.str() ;
}

} // end of namespace

namespace std
{
	void swap( pdf::Object& obj1, pdf::Object& obj2 )
	{
		obj1.Swap( obj2 ) ;
	}
}
