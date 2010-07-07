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

/**	\file	NameTree.cc
	\brief	implementation of the NameTree class
	\date	Feb 4, 2010
	\author	Nestal Wan
*/

#include "NameTree.hh"

#include "File.hh"
#include "DictReader.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"

#include "util/CArray.hh"
#include "util/Functional.hh"

#include <boost/bind.hpp>

#include <algorithm>

namespace pdf {

NameTree::NameTree( )
{
}

/**	constructor
	
*/
NameTree::NameTree( DictReader& self, File *file )
{
	Read( self, file ) ;
}

const Object& NameTree::LookUp( const std::string& key ) const
{
	NameMap::const_iterator i = m_names.find( key ) ;
	return i != m_names.end() ? i->second : Object::NullObj() ;
}

void NameTree::Add( const std::string& key, const Object& val )
{
	m_names.insert( std::make_pair(key, val) ) ;
}

NameTree::~NameTree( )
{
	std::for_each( m_kids.begin(), m_kids.end(), DeletePtr() ) ; 
}

Ref NameTree::Write( File *file ) const
{
	Dictionary self ;
	
	// write the kids and get the Refs in an array
	if ( !m_kids.empty() )
	{
		Array kids( m_kids.size() ) ;
		std::transform( m_kids.begin(), m_kids.end(), kids.begin(),
			boost::bind( &NameTree::Write, _1, file ) ) ;
		
		self.insert( "Kids", kids ) ;
	}
	if ( !m_names.empty() )
	{
		Array names ;
		for ( NameMap::const_iterator i = m_names.begin() ; i != m_names.end() ;
			++i )
		{
			names.push_back( i->first ) ;
			names.push_back( i->second ) ;
		}
		self.insert( "Names", names ) ;
	}
	
	if ( m_names.size() == 1 )
		self.insert( "Limits", Array( 2, m_names.begin()->first ) ) ;
	else if ( m_names.size() > 1 )
	{
		Object limits[] = { m_names.begin()->first, m_names.rbegin()->first } ;
		self.insert( "Limits", Array( Begin(limits), End(limits) ) ) ;
	}
	
	return file->WriteObj( self ) ;
}

void NameTree::Read( DictReader& self, File *file )
{
	Array kids ;
	if ( self.Detach( "Kids", kids ) )
	{
		for ( Array::iterator i = kids.begin() ; i != kids.end() ; ++i )
		{
			// kids must be indirect objects
			Dictionary kid_self = file->ReadObj( *i ).As<Dictionary>() ;
			DictReader kid_reader( kid_self, file ) ;
			m_kids.push_back( new NameTree( kid_reader, file ) ) ;
		}
	}
	
	Array names ;
	if ( self.Detach( "Names", names ) )
	{
		Array::iterator it = names.begin() ;
		while ( it != names.end() )
		{
			std::string key ;
			key.swap( it->As<std::string>() ) ;
		
			it++ ;
			if ( it != names.end() )
			{
				it->Swap( m_names[key] ) ;
				it++ ;
			}
		}
	}
}

} // end of namespace
