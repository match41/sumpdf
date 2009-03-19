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
	\file	DeRefVisitor.hh
	\brief	definition the DeRefVisitor class
	\date	Sun May 25 2008
	\author	Nestal Wan
*/

#ifndef __PDF_DE_REF_VISITOR_HEADER_INCLUDED__
#define __PDF_DE_REF_VISITOR_HEADER_INCLUDED__

#include "IElementSrc.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "core/Object.hh"

#include "util/Util.hh"

#include <boost/variant/static_visitor.hpp>
#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>

namespace pdf {

class IElementSrc ;

/*!	\brief	brief description
	
	this class represents
*/
class DeRefVisitor : public boost::static_visitor<>
{
public :
	DeRefVisitor( IElementSrc *file ) : m_file( file )
	{
	}
	
	void operator()( Array& array )
	{
		std::for_each( array.begin( ), array.end( ),
		               boost::bind( &DeRefVisitor::Dereference, this, _1 ) ) ;
	}

	void operator()( Dictionary& dict )
	{
		for ( Dictionary::iterator i  = dict.begin( ) ;
		                           i != dict.end( ) ; ++i )
			Dereference( i->second ) ;
	}
	
	template <typename T>
	void operator()( T& t )
	{
std::cerr << "wa" ;
	}

private :
	void Dereference( Object& obj )
	{
		if ( obj.Type( ) == Object::ref )
			obj = m_file->ReadObj( obj, true ) ;
	}

private :
	IElementSrc	*m_file ;
} ;

} // end of namespace

#endif
