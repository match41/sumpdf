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
	\file	Catalog.cc
	\brief	implementation the Catalog class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#include "Catalog.hh"

#include "IElementSrc.hh"
#include "IElementDest.hh"
#include "ElementList.hh"

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
	: m_tree( 0 )
{
}

void Catalog::Read( const Ref& link, IElementSrc *file )
{
	assert( file != 0 ) ;
	m_self = file->ReadObj( link ) ;

	if ( m_self["Type"].As<Name>() != "Catalog" )
	{
		std::ostringstream oss ;
		oss << "invalid catalog type: " << m_self["Type"] ;
		throw ParseError( oss.str( ) ) ;
	}
	
	// page tree is manatory
	m_tree = file->Read<PageTree>( m_self["Pages"] ) ;
	m_self.erase( "Pages" ) ;
	
	// TODO: no know how to handle it yet
	m_self.erase( Name( "OpenAction" ) ) ;
}

void Catalog::Write( const Ref& link, IElementDest *dest ) const
{
	Dictionary self( m_self ) ;
	self["Pages"] 	    = dest->Write( m_tree ) ;
	self["Type"]	    = Name( "Catalog" ) ;
	self["PageLayout"]	= Name( "OneColumn" ) ;
	
	dest->WriteObj( self, link ) ;
}

void Catalog::AddPage( RealPage *page )
{
	if ( m_tree == 0 )
		m_tree = new PageTree ;
	
	// for now the pages are arranged linearly for now
	m_tree->AddLeaf( page ) ;
}

std::size_t Catalog::PageCount( ) const
{
	return m_tree->Count( ) ;
}

ElementList Catalog::GetChildren( ) const
{
	IElement *list[] = { m_tree } ;
	return ElementList( list, list + 1 ) ;
}

RealPage* Catalog::GetPage( std::size_t index )
{
	PageNode *p = m_tree->GetLeaf( index ) ;
	assert( typeid(*p) == typeid(RealPage) ) ;
	
	return static_cast<RealPage*>( p ) ;
}

} // end of namespace
