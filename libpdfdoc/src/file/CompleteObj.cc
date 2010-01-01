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
#include "util/RefCounterWrapper.hh"

#include <boost/bind.hpp>

namespace pdf {

/**	constructor
*/
CompleteObj::CompleteObj( )
{
}

void CompleteObj::Read( Object& obj, IFile *file )
{
	assert( file != 0 ) ;

	m_obj.Swap( obj ) ;
	
	file->ReadObjectLinks( m_obj, m_refs ) ;
}

void CompleteObj::ReplaceReference( Object& obj, IFile *file ) const
{
	assert( file != 0 ) ;
	
	if ( obj.IsType<Ref>() )
	{
		ObjMap::const_iterator i = m_refs.find( obj ) ;
		assert( i != m_refs.end() ) ;
		assert( i->second != 0 ) ;
		
		obj = file->WriteObj( i->second->Get( ) ) ;
	}
}

Ref CompleteObj::Write( IFile *file ) const
{
	assert( file != 0 ) ;

	Object obj( m_obj ) ;
	ForEachObj( obj,
		boost::bind(
			&CompleteObj::ReplaceReference,
			this,
			_1,
			file ) ) ; 
	
	return file->WriteObj( obj ) ;
}

} // end of namespace
