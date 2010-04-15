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

/*!
	\file	PageNode.hh
	\brief	definition the PageNode class
	\date	Sat Apr 12 2008
	\author	Nestal Wan
*/

#ifndef __PDF_PAGE_NODE_HEADER_INCLUDED__
#define __PDF_PAGE_NODE_HEADER_INCLUDED__

#include "page/Page.hh"

#include "util/RefCounter.hh"
#include "util/Rect.hh"

#include <cstddef>

namespace pdf {

class DictReader ;
class File ;
class FontSubsetInfo ;
class Ref ;

/*!	\brief	brief description

	this class represents the PageNode class, which is a node in the page tree.
	it can be a leaf node (i.e. a page) or a tree node (which contains other
	nodes)
*/
class PageNode : public RefCounter
{
public :
	virtual ~PageNode( ) ;

	virtual void Read( DictReader& dict ) = 0 ; 
	virtual void Write( 
		const Ref& 				link,
		File 					*file,
		const Ref& 				parent,
		const FontSubsetInfo	*ss ) const = 0 ;

	virtual PageNode* Parent( ) = 0 ;

	virtual std::size_t Count( ) const = 0 ;
	virtual PageNode* GetLeaf( std::size_t index ) = 0 ;
	
	// page boundaries
	virtual Rect CropBox( ) const = 0;
	virtual Rect MediaBox( ) const = 0 ;
} ;

} // end of namespace

#endif
