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

/**	\file	Function.cc
	\brief	implementation of the Function class
	\date	Mar 4, 2010
	\author	Nestal Wan
*/

#include "Function.hh"

#include "core/Object.hh"
#include "core/Array.hh"
#include "core/Dictionary.hh"

#include "file/DictReader.hh"

#include "stream/Stream.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"

#include <vector>
#include <iostream>

namespace pdf {

struct Function::Impl
{
	// type of the function
	int	type ;
	
	std::vector<double>	domain ;
	std::vector<double>	range ;
	
	// type 1 functions
	std::vector<int>	size ;
	int		bits_per_sample ;
	int		order ;
	std::vector<double>	encode ;
	std::vector<double>	decode ;
	
	// type 2 functions
	std::vector<double>	c0 ;
	std::vector<double>	c1 ;
	double				exponent ;
	
	// type 3 functions
	std::vector<Function>	functions ;
	std::vector<double>		bounds ;

	// type 4 only use data
	Stream	data ;
} ;

/**	constructor
	
*/
Function::Function( )
: m_impl( new Impl )
{
}

void Function::Read( Object& obj, File *file )
{
	PDF_ASSERT( file != 0 ) ;

	if ( obj.Is<Stream>() )
	{
		Stream& str = obj.As<Stream>() ;

		Dictionary dict = str.Self() ;
		DictReader reader( dict, file ) ;
		ReadCommon( reader ) ;
	
		if ( m_impl->type == 0 )
			ReadType0( reader, str ) ;
	}
}

void Function::ReadType0( DictReader& dict, Stream& data )
{
	Array size ;
	if ( dict.Detach( "Size", size ) )
		m_impl->size.assign( size.begin(), size.end() ) ;
	
	// BitsPerSample is required
	if ( !dict.Detach( "BitsPerSample", m_impl->bits_per_sample ) )
		throw ParseError( "unknown function bits per sample" ) ;
	
	// Order is optional. default value is 1.
	if ( !dict.Detach( "Order", m_impl->order ) )
		m_impl->order = 1 ;

	dict.Detach( "Encode", m_impl->encode ) ;
	dict.Detach( "Decode", m_impl->decode ) ;
	
	data.ClearDictionary() ;
	m_impl->data = data ;
}

void Function::ReadCommon( DictReader& dict )
{
	// type is required
	if ( !dict.Detach( "FunctionType", m_impl->type ) )
		throw ParseError( "unknown function type" ) ;

	// domain is also required
	if ( !dict.Detach( "Domain", m_impl->domain ) )
		throw ParseError( "unknown function domain" ) ;
		
	// range is required for type 0 and 4 functions
	if ( !dict.Detach( "Range", m_impl->range ) &&
	     (m_impl->type == 0 || m_impl->type == 4)  )
		throw ParseError( "unknown function range" ) ;
}

} // end of namespace
