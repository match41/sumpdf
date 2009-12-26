/***************************************************************************\
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
\***************************************************************************/

/*!
	\file	PageNode.hh
	\brief	definition the PageNode class
	\date	Sat Apr 12 2008
	\author	Nestal Wan
*/

#ifndef __PDF_PAGE_NODE_HEADER_INCLUDED__
#define __PDF_PAGE_NODE_HEADER_INCLUDED__

#include <cstddef>

namespace pdf {

class Dictionary ;
class PageTree ;
class Resources ;
class IFile ;
class Ref ;

/*!	\brief	brief description

	this class represents the PageNode class, which is a node in the page tree.
	it can be a leaf node (i.e. a page) or a tree node (which contains other
	nodes)
*/
class PageNode
{
public :
	PageNode( ) ;
	virtual ~PageNode( ) ;

	virtual void Read( const Dictionary& self, IFile *file ) = 0 ; 
	virtual void Write( const Ref& link, IFile *file, const Ref& parent ) 
		const = 0 ;

	virtual PageTree* Parent( ) = 0 ;

	Resources* GetResource( ) ;
	const Resources* GetResource( ) const ;

	virtual std::size_t Count( ) const = 0 ;
	virtual PageNode* GetLeaf( std::size_t index ) = 0 ;

private :
	Resources	*m_resources ;
} ;

} // end of namespace

#endif
