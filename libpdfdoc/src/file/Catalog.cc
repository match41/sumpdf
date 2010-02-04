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
#include "File.hh"
#include "NameTree.hh"

#include "core/Array.hh"
#include "core/Ref.hh"

#include "page/RealPage.hh"
#include "page/PageTree.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/Util.hh"

#include <iostream>

namespace pdf {

struct Catalog::NameDict
{
	NameTree	m_dests, m_ap, m_javascript, m_pages, m_temps, m_ids, m_urls,
				m_emb_files, m_alt_present, m_rend ;

	void Read( Dictionary& self, File *file )
	{
		Dictionary dests ;
		if ( Detach( file, self, "Dests", dests ) )
			m_dests.Read( dests, file ) ;
	}
} ;

Catalog::Catalog( FT_Library ft_lib )
	: m_version		( "1.4" ),
	  m_page_layout	( "SinglePage" ),
	  m_page_mode	( "UseNode" ),
	  m_tree		( new PageTree( ft_lib ) ),
	  m_name_dict	( new NameDict )
{
}

Catalog::Catalog( const Ref& link, File *file, FT_Library ft_lib )
	: m_version		( "1.4" ),
	  m_page_layout	( "SinglePage" ),
	  m_page_mode	( "UseNode" ),
	  m_tree		( 0 ),
	  m_name_dict	( new NameDict )
{
	PDF_ASSERT( file != 0 ) ;
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
	m_tree = new PageTree( ft_lib ) ;
	m_tree->Read( tree, file ) ;
	
	Detach( file, self, "Version",		m_version ) ;
	Detach( file, self, "PageLayout",	m_page_layout ) ;
	Detach( file, self, "PageMode",		m_page_mode ) ;

	// read destintions
	Dictionary dest ;
	if ( Detach( file, self, "Dests", dest ) )
	{
		for ( Dictionary::iterator i = dest.begin() ; i != dest.end() ; ++i )
		{
			Array darray = DeRefObj<Array>( file, i->second ) ; 
			
			Destination d ;
			d.Read( darray, file ) ;
			
			m_named_dests.insert( std::make_pair( i->first, d ) ) ;
		}
	}

	Dictionary name_dict ;
	if ( Detach( file, self, "Names", name_dict ) )
		m_name_dict->Read( name_dict, file ) ;
		
	std::cout << "catalog: " << self << std::endl ;
}

Catalog::~Catalog( )
{
	// clear the destination map first. it should free up all links to
	// the pages.
	m_named_dests.clear( ) ;

	// there should be no one linking to the root tree node now.
	PDF_ASSERT( m_tree->UseCount() == 1 ) ;
	
	m_tree->Release() ;
	m_tree = 0 ;
}

Ref Catalog::Write( File *file ) const
{
	Dictionary self ;

	Ref tree = file->AllocLink( ) ;
	m_tree->Write( tree, file, Ref() ) ; 

	self["Pages"] 	    = tree ;
	self["Type"]	    = Name( "Catalog" ) ;
	self["Version"]	= m_version ;
	self["PageLayout"]	= m_page_layout ;
	self["PageMode"]	= m_page_mode ;

	// write destinations
	Dictionary dest ;
	for ( std::map<Name, Destination>::const_iterator i = m_named_dests.begin() ;
		i != m_named_dests.end() ; ++i )
	{
		dest.insert( std::make_pair( i->first, i->second.Write( file ) ) ) ;
	}
	if ( !dest.empty() )
		self["Dests"]		= file->WriteObj(dest) ;

	return file->WriteObj( self ) ;
}

RealPage* Catalog::AddPage( )
{
	PDF_ASSERT( m_tree != 0 ) ;

	// for now the pages are arranged linearly for now
	return new RealPage( m_tree ) ;
}

std::size_t Catalog::PageCount( ) const
{
	PDF_ASSERT( m_tree != 0 ) ;

	return m_tree->Count( ) ;
}

RealPage* Catalog::GetPage( std::size_t index )
{
	PDF_ASSERT( m_tree != 0 ) ;

	PageNode *p = m_tree->GetLeaf( index ) ;
	PDF_ASSERT( p != 0 ) ;
	
	// no need to use dynamic cast as it will not be child class of RealPage
	PDF_ASSERT( typeid(*p) == typeid(RealPage) ) ;

	return static_cast<RealPage*>( p ) ;
}

Resources* Catalog::GetResource( )
{
	PDF_ASSERT( m_tree != 0 ) ;
	return m_tree->GetResource() ;
}

} // end of namespace
