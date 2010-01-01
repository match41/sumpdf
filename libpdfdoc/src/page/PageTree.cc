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

#include "core/Array.hh"
#include "core/Dictionary.hh"

#include "file/ObjectReader.hh"
#include "file/IFile.hh"

#include "util/Exception.hh"
#include "util/Util.hh"

#include <boost/bind.hpp>
#include <boost/lambda/construct.hpp>

#include <cassert>
#include <numeric>

#include <iostream>

namespace pdf {

PageTree::PageTree( PageTree *parent )
	: m_parent( parent ),
	  m_count( 0 )
{
	if ( parent )
		parent->AppendNode( this ) ;
}

PageTree::~PageTree( )
{
	std::for_each( m_kids.begin(), m_kids.end(),
	               boost::lambda::delete_ptr( ) ) ;
}

void PageTree::Read( Dictionary& dict, IFile *file )
{
	Array pages ;
	if ( !Detach( file, dict, "Kids", pages ) )
		throw ParseError( "no children in page tree" ) ;
		
	for ( Array::iterator i = pages.begin() ; i != pages.end() ; ++i )
	{
		Dictionary d = DeRefObj<Dictionary>( file, *i ) ;
		const Name& type = d["Type"].As<Name>() ; 
		
		if ( type == Name( "Pages" ) )
		{
			PageTree *p = new PageTree( this ) ;
			p->Read( d, file ) ;
		}
		
		else if ( type == Name( "Page" ) )
		{
			RealPage *p = new RealPage( this ) ;
			p->Read( d, file ) ;
		}
		else
			throw ParseError( "invalid page type" ) ;
	}

	// leaf count is required
	if ( !Detach( file, dict, "Count", m_count ) )
		throw ParseError( "cannot get leaf count in page node" ) ;
	
	Dictionary res ;
	if ( Detach( file, dict, "Resources", res ) )
		m_resources.Read( res, file ) ;
}

void PageTree::Write( const Ref& link, IFile *file, const Ref& ) const
{
	assert( file != 0 ) ;

	std::vector<Ref> kids ;
	for ( std::vector<PageNode*>::const_iterator i  = m_kids.begin() ;
	                                             i != m_kids.end() ; ++i )
	{
		Ref child = file->AllocLink( ) ;
		(*i)->Write( child, file, link ) ;
		kids.push_back( child ) ;
	}

	// update page count before writing
	UpdateCount( ) ;

	double mbox[] = { 0, 0, 595.1, 842.1 } ;
	Dictionary self ;
	self["Type"]		= Name( "Pages" ) ;
	self["Kids"]		= Array( kids.begin( ), kids.end( ) ) ;
	self["Count"]		= m_count ;
	self["MediaBox"]	= Array( Begin( mbox ), End( mbox ) ) ;
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

Resources* PageTree::GetResource( )
{
	return &m_resources ;
}

const Resources* PageTree::GetResource( ) const
{
	return &m_resources ;
}

} // end of namespace
