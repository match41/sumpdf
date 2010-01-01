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
	\file	Catalog.cc
	\brief	implementation the Catalog class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#include "Catalog.hh"

#include "ObjectReader.hh"
#include "IFile.hh"

#include "core/Array.hh"
#include "core/Ref.hh"

#include "page/RealPage.hh"
#include "page/PageTree.hh"
#include "util/Util.hh"
#include "util/Exception.hh"

#include <cassert>

#include <iostream>

namespace pdf {

Catalog::Catalog( )
: m_version		( "1.4" ),
  m_page_layout	( "SinglePage" ),
  m_page_mode	( "UseNode" ),
  m_tree		( 0 )
{
}

Catalog::Catalog( const Ref& link, IFile *file )
	: m_version		( "1.4" ),
	  m_page_layout	( "SinglePage" ),
	  m_page_mode	( "UseNode" ),
	  m_tree		( 0 )
{
	assert( file != 0 ) ;
	Dictionary self = file->ReadObj( link ).As<Dictionary>() ;

	Name type ;
	if ( !Detach( file, self, "Type", type ) || type != "Catalog" )
	{
		std::ostringstream oss ;
		oss << "invalid catalog type: " << type ;
		throw ParseError( oss.str( ) ) ;
	}

	// page tree is mandatory
	Dictionary tree ;
	if ( !Detach( file, self, "Pages", tree ) )
		throw ParseError( "no page tree in catalog" ) ;
	
	// root page tree has no parent
	m_tree = new PageTree ;
	m_tree->Read( tree, file ) ;
	
	Detach( file, self, "Version",		m_version ) ;
	Detach( file, self, "PageLayout",	m_page_layout ) ;
	Detach( file, self, "PageMode",		m_page_mode ) ;
	
	m_self.Read( self, file ) ;
}

Catalog::~Catalog( )
{
	delete m_tree ;
}

Ref Catalog::Write( IFile *file ) const
{
	CompleteObj self( m_self ) ;

	Ref tree = file->AllocLink( ) ;
	m_tree->Write( tree, file, Ref() ) ; 

	self.Get()["Pages"] 	    = tree ;
	self.Get()["Type"]	    	= Name( "Catalog" ) ;
	self.Get()["Version"]		= m_version ;
	self.Get()["PageLayout"]	= m_page_layout ;
	self.Get()["PageMode"]		= m_page_mode ;

	return self.Write( file ) ;
}

RealPage* Catalog::AddPage( )
{
	if ( m_tree == 0 )
		m_tree = new PageTree ;

	// for now the pages are arranged linearly for now
	return new RealPage( m_tree ) ;
}

std::size_t Catalog::PageCount( ) const
{
	return m_tree->Count( ) ;
}

RealPage* Catalog::GetPage( std::size_t index )
{
	PageNode *p = m_tree->GetLeaf( index ) ;
	assert( typeid(*p) == typeid(RealPage) ) ;

	return static_cast<RealPage*>( p ) ;
}

} // end of namespace
