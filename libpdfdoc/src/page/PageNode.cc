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
	\file	PageNode.cc
	\brief	implementation the PageNode class
	\date	Sat Apr 12 2008
	\author	Nestal Wan
*/

#include "PageNode.hh"
#include "PageTree.hh"

#include "Resources.hh"

#include "core/Object.hh"
#include "file/IElementSrc.hh"
#include "file/ElementList.hh"

#include <iostream>
#include <cassert>

namespace pdf {

PageNode::PageNode( )
	: m_parent( 0 ),
	  m_resources( new Resources )
{
}

PageNode::PageNode( PageTree *parent )
	: m_parent( parent ),
	  m_resources( new Resources )
{
	assert( parent != this ) ;
}

PageTree* PageNode::Parent( )
{
	return m_parent ;
}

const PageTree* PageNode::Parent( ) const
{
	return m_parent ;
}

void PageNode::SetParent( PageTree *parent )
{
	m_parent = parent ;
}

Resources* PageNode::GetResource( )
{
	assert( m_resources != 0 ) ;
	return m_resources ;
}

const Resources* PageNode::GetResource( ) const
{
	assert( m_resources != 0 ) ;
	return m_resources ;
}

void PageNode::ReadResource( const Object& robj, IElementSrc *repo )
{
	// resources may not always be indirect objects
	delete m_resources ;
	m_resources = ( robj.Type() == Object::ref ? repo->Read<Resources>( robj )
		                        : new Resources( robj.As<Dictionary>(), repo ));
}

ElementList PageNode::GetChildren( ) const
{
	assert( m_resources != 0 ) ;
	ElementList l ;
	l.push_back( m_resources ) ;
	return l ;
}

} // end of namespace
