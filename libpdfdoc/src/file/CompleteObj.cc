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
#include "ResourcePool.hh"

#include "core/TraverseObject.hh"
#include "core/ObjWrapper.hh"
#include "util/Exception.hh"
#include "util/RefCounterWrapper.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>

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

void CompleteObj::ReplaceReference(
	Object& obj,
	IFile *file,
	std::map<ObjWrapper*, Ref>& written ) const
{
	assert( file != 0 ) ;
	
	if ( obj.IsType<Ref>() )
	{
		ObjMap::const_iterator i = m_refs.find( obj ) ;
		if ( i != m_refs.end() )
		{
			assert( i->second != 0 ) ;
			
			std::map<ObjWrapper*, Ref>::iterator j = written.find( i->second ) ;
			if ( j == written.end() )
			{
				ObjectPool *pool = &file->Pool( )->objs ;
				Ref link = pool->Find( i->second ) ;
				if ( link == Ref() )
				{
					link = file->AllocLink( ) ;
std::cout << "link = " << link << " " << i->second << std::endl ;

					written.insert( std::make_pair( i->second, link ) ) ;
					pool->Add( link, i->second ) ;
	
					Object temp = i->second->Get( ) ;
					ReplaceChildReference( temp, file, written ) ;
					file->WriteObj( temp, link ) ;
				}
				else
					written.insert( std::make_pair( i->second, link ) ) ;
				
				obj = link ;
			}
			else
			{
std::cout << "link2 = " << j->second << i->second << std::endl ;
				obj = j->second ;
			}
		}
	}
}

Ref CompleteObj::Write( IFile *file ) const
{
	assert( file != 0 ) ;
	
	std::map<ObjWrapper*, Ref> written ;
	Object obj( m_self ) ;
	ReplaceChildReference( obj, file, written ) ;
	return file->WriteObj( obj ) ;
}

void CompleteObj::Write( IFile *file, const Ref& link ) const
{
	assert( file != 0 ) ;
	
	std::map<ObjWrapper*, Ref> written ;
	Object obj( m_self ) ;
	ReplaceChildReference( obj, file, written ) ;
	file->WriteObj( obj, link ) ;
}

void CompleteObj::ReplaceChildReference(
	Object& obj,
	IFile *file,
	std::map<ObjWrapper*, Ref>& written ) const
{
	ForEachObj( obj,
		boost::bind(
			&CompleteObj::ReplaceReference,
			this,
			_1,
			file,
			written) ) ; 
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
