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

/**
	\file	CompleteObj.cc
	\brief	implementation of the CompleteObj class
	\date	Jan 1, 2010
	\author	Nestal Wan
*/

#include "CompleteObj.hh"

#include "IFile.hh"

#include "core/TraverseObject.hh"
#include "core/ObjWrapper.hh"
#include "util/Exception.hh"
#include "util/RefCounterWrapper.hh"

#include <boost/bind.hpp>

#include <algorithm>
//#include <iostream>

namespace pdf {

/**	constructor
*/
CompleteObj::CompleteObj( )
{
}

CompleteObj::CompleteObj( const CompleteObj& other )
	: m_self( other.m_self ),
	  m_refs( other.m_refs )
{
	std::for_each(
		m_refs.begin(),
		m_refs.end(),
		boost::bind( &ObjWrapper::AddRef,
			boost::bind( &ObjMap::value_type::second,
				_1 ) ) ) ;
}

CompleteObj::~CompleteObj( )
{
	std::for_each(
		m_refs.begin(),
		m_refs.end(),
		boost::bind( &ObjWrapper::Release,
			boost::bind( &ObjMap::value_type::second,
				_1 ) ) ) ;
}

void CompleteObj::Read( Dictionary& dict, IFile *file )
{
	assert( file != 0 ) ;

	m_self.swap( dict ) ;
	m_refs.clear( ) ;

	file->ReadObjectLinks( m_self, m_refs ) ;
}

void CompleteObj::ReplaceReference( Object& obj, IFile *file ) const
{
	assert( file != 0 ) ;
	
	if ( obj.IsType<Ref>() )
	{
		ObjMap::const_iterator i = m_refs.find( obj ) ;
		if ( i != m_refs.end() )
		{
			assert( i->second != 0 ) ;
			
			obj = file->WriteObj( i->second->Get( ) ) ;
		}
	}
}

Ref CompleteObj::Write( IFile *file ) const
{
	assert( file != 0 ) ;

	Object obj( m_self ) ;
	ForEachObj( obj,
		boost::bind(
			&CompleteObj::ReplaceReference,
			this,
			_1,
			file ) ) ; 
	
	return file->WriteObj( obj ) ;
}

const Object& CompleteObj::Find( const Ref& link ) const
{
	ObjMap::const_iterator i = m_refs.find( link ) ;
	
	assert( i == m_refs.end() || i->second != 0 ) ; 	
	return i != m_refs.end() ? i->second->Get() : Object::NullObj() ;
}

Dictionary& CompleteObj::Get( )
{
	return m_self ;
}

const Dictionary& CompleteObj::Get( ) const
{
	return m_self ;
}

} // end of namespace
