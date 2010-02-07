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
	\file	PageTree.cc
	\brief	implementation the PageTree class
	\date	Sat Apr 12 2008
	\author	Nestal Wan
*/

#include "PageTree.hh"

#include "RealPage.hh"
#include "RealResources.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"

#include "file/ArrayReader.hh"
#include "file/DictReader.hh"
#include "file/File.hh"
#include "file/ElementPool.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>
#include <numeric>

#include <iostream>

namespace pdf {

PageTree::PageTree( PageTree *parent )
	: m_parent( parent ),
	  m_resources( new RealResources(parent == 0 ? 0 : parent->GetResource()) ),
	  m_count( 0 )
{
	PDF_ASSERT( parent != 0 ) ;
	if ( parent )
		parent->AppendNode( this ) ;
}

PageTree::PageTree( FT_LibraryRec_ *ft )
	: m_parent( 0 ),
	  m_resources( new RealResources( ft ) ),
	  m_count( 0 )
{
}

PageTree::~PageTree( )
{
	std::for_each(
		m_kids.begin(),
		m_kids.end(),
	    boost::mem_fn( &PageNode::Release ) ) ;
	m_resources->Release( ) ;
}

void PageTree::Read( DictReader& dict )
{
	File *file = dict.GetFile() ;
	
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( file->Pool() != 0 ) ;

	ArrayReader pages ;
	if ( !dict.Detach( "Kids", pages ) )
		throw ParseError( "no children in page tree" ) ;

	ElementPool *pool = file->Pool( ) ;
	for ( std::size_t i = 0 ; i < pages->size() ; ++i )
	{
		DictReader d ;
		pages.Detach<DictReader>( i, d ) ;
		Name type = d.At<Name>("Type") ; 
		
		PageNode *p = 0 ;
		if ( type == "Pages" )
		{
			p = new PageTree( this ) ;
			p->Read( d ) ;
		}
		
		else if ( type == "Page" )
		{
			p = new RealPage( this ) ;
			p->Read( d ) ;
		}
		else
			throw ParseError( "invalid page type" ) ;
		
		PDF_ASSERT( p != 0 ) ;
		
		if ( pages[i].Is<Ref>() )
			pool->Add( pages[i], p ) ;
	}

	// leaf count is required
	if ( !dict.Detach( "Count", m_count ) )
		throw ParseError( "cannot get leaf count in page node" ) ;

	Ref link = dict.At<Ref>("Resources" ) ;
	if ( !pool->Acquire( link, m_resources ) )  
	{
		DictReader res_dict ;
		if ( dict.Detach( "Resources", res_dict ) )
		{
			m_resources->Read( res_dict ) ;
			pool->Add( link, m_resources ) ; 
		}
	}
}

void PageTree::Write( const Ref& link, File *file, const Ref& ) const
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( file->Pool() != 0 ) ;
	ElementPool *pool = file->Pool() ;  

	std::vector<Ref> kids ;
	for ( std::vector<PageNode*>::const_iterator i  = m_kids.begin() ;
	                                             i != m_kids.end() ; ++i )
	{
		Ref child = file->AllocLink( ) ;
		(*i)->Write( child, file, link ) ;
		kids.push_back( child ) ;
		pool->Add( child, *i ) ;
	}

	// update page count before writing
	UpdateCount( ) ;

	double mbox[] = { 0, 0, 595.1, 842.1 } ;
	Dictionary self ;
	self["Type"]		= Name( "Pages" ) ;
	self["Kids"]		= Array( kids.begin( ), kids.end( ) ) ;
	self["Count"]		= m_count ;
	self["MediaBox"]	= Array( Begin( mbox ), End( mbox ) ) ;
	
	Ref ref = pool->Find( m_resources ) ;
	if ( ref == Ref() )
	{
		ref = m_resources->Write( file ) ;
		pool->Add( ref, m_resources ) ;
	}
	
	self["Resources"]	= ref ;
	
	file->WriteObj( self, link ) ;
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
	
	m_kids.insert( m_kids.begin() + index, child ) ;
}

void PageTree::AppendNode( PageNode *child )
{
	assert( child != 0 ) ;
	
	m_kids.push_back( child ) ;
}

PageTree* PageTree::Parent( )
{
	return m_parent ;
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

RealResources* PageTree::GetResource( )
{
	PDF_ASSERT( m_resources != 0 ) ;
	return m_resources ;
}

const RealResources* PageTree::GetResource( ) const
{
	PDF_ASSERT( m_resources != 0 ) ;
	return m_resources ;
}

Font* PageTree::CreateSimpleFont( const std::string& name )
{
	PDF_ASSERT( m_resources != 0 ) ;
	return m_resources->CreateSimpleFont( name ) ;
}

} // end of namespace
