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
	\file	PageTree.cc
	\brief	implementation the PageTree class
	\date	Sat Apr 12 2008
	\author	Nestal Wan
*/

#include "PageTree.hh"

#include "RealPage.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"

#include "file/IElementSrc.hh"
#include "file/IElementDest.hh"
#include "file/IFile.hh"
#include "file/ElementList.hh"

#include "util/Util.hh"

#include <boost/bind.hpp>

#include <cassert>
#include <numeric>
#include <stdexcept>

#include <iostream>

namespace pdf {

PageTree::PageTree( )
	: PageNode( ),
	  m_count( 0 )
{
}

PageTree::PageTree( PageTree *parent )
	: PageNode( parent ),
	  m_count( 0 )
{
	assert( parent != this ) ;
	if ( parent )
		parent->AppendNode( this ) ;
}

PageNode* PageTree::ReadNode( const Ref& link, IElementSrc *repo )
{
	Dictionary d = repo->ReadObj( link ) ;
	if ( d["Type"].As<Name>() == Name( "Pages" ) )
		return repo->Read<PageTree>( link ) ;
	
	else if ( d["Type"].As<Name>() == Name( "Page" ) )
		return repo->Read<RealPage>( link ) ;
	
	else
		throw std::runtime_error( "invalid page type" ) ;
}

void PageTree::Init( Object& obj, IElementSrc *repo )
{
	Dictionary d ;
	std::swap( d, obj.As<Dictionary>() ) ;
	
	using namespace boost ;

	const Array& pages = d["Kids"].As<Array>() ;
	std::transform( pages.begin( ), pages.end( ),
					std::back_inserter( m_kids ),
					bind( &PageTree::ReadNode, _1, repo ) ) ;

	// leaf count is required
	m_count	= d["Count"].As<int>( ) ;
}

void PageTree::Write( const Ref& link, IElementDest *repo ) const
{
	using namespace boost ;
	
	std::vector<Ref> kids ;
	std::transform( m_kids.begin( ), m_kids.end( ),
	                std::back_inserter( kids ),
	                bind( &IElementDest::Write, repo, _1 ) ) ;

	// update page count before writing
	UpdateCount( ) ;

	double mbox[] = { 0, 0, 595.1, 842.1 } ;
	Dictionary self ;
	self["Type"]		= Name( "Pages" ) ;
	self["Kids"]		= Array( kids.begin( ), kids.end( ) ) ;
	self["Count"]		= m_count ;
	self["MediaBox"]	= Array( Begin( mbox ), End( mbox ) ) ;
	repo->WriteObj( self, link ) ;
}

void PageTree::IncChildCount( )
{
	// increment count in all tree nodes include myself
	PageTree *tree = this ;
	while ( tree != 0 )
	{
		tree->m_count++ ;
		tree = tree->Parent( ) ;
	}
}

/*! \brief  append the leaf node at the end of the tree node
*/
void PageTree::AppendLeaf( RealPage *child )
{
	assert( child != 0 ) ;
    IncChildCount( ) ;
	AppendNode( child ) ;
}

void PageTree::AddLeaf( std::size_t index, RealPage *child )
{
	assert( child != 0 ) ;
    IncChildCount( ) ;
	
	child->SetParent( this ) ;
	m_kids.insert( m_kids.begin() + index, child ) ;
}

void PageTree::AppendNode( PageNode *child )
{
	assert( child != 0 ) ;
	
	child->SetParent( this ) ;
	m_kids.push_back( child ) ;
}

std::size_t PageTree::Count( ) const
{
	return m_count ;
}

void PageTree::UpdateCount( ) const
{
	using namespace boost ;
	m_count = std::accumulate( m_kids.begin( ), m_kids.end( ), 0,
	                           bind( std::plus<std::size_t>(), _1,
	                                 bind( &PageNode::Count, _2 ) ) ) ;
}

ElementList PageTree::GetChildren( ) const
{
	return ElementList( m_kids.begin( ), m_kids.end( ) ) ;
}

PageNode* PageTree::GetLeaf( std::size_t index )
{
	std::size_t current = 0 ;
	for ( std::vector<PageNode*>::iterator i  = m_kids.begin( ) ;
	                                       i != m_kids.end( ) ; ++i )
	{
		assert( *i != 0 ) ;
	
		if ( index < current + (*i)->Count( ) )
			return (*i)->GetLeaf( index - current ) ;
	
		current += (*i)->Count( ) ;
	}
	
	return 0 ;
}

} // end of namespace
