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
	\file	PageTree.hh
	\brief	definition the PageTree class
	\date	Sat Apr 12 2008
	\author	Nestal Wan
*/

#ifndef __PDF_PAGE_TREE_HEADER_INCLUDED__
#define __PDF_PAGE_TREE_HEADER_INCLUDED__

#include "PageNode.hh"
#include "PageInfo.hh"

#include <vector>
#include <string>

namespace pdf {

class RealPage ;
class RealResources ;
class Dictionary ;
class File ;
class Font ;
class FontDb ;

/*!	\brief	tree nodes in page tree
	
	this class represents
*/
class PageTree : public PageNode
{
public :
	explicit PageTree( PageTree *parent ) ;
	explicit PageTree( FontDb *fontdb ) ;
	~PageTree( ) ;

	void Read( DictReader& dict ) ;
	void Write( const Ref& link, File *file, const Ref& parent ) const ;

	PageTree* Parent( ) ;
	std::size_t Count( ) const ;

	PageNode* GetLeaf( std::size_t index ) ;
	void AppendLeaf( RealPage *child ) ;
	void AddLeaf( std::size_t index, RealPage *child ) ;
	RealResources* GetResource( ) ;
	const RealResources* GetResource( ) const ;
	
	Rect MediaBox( ) const ;
	Rect CropBox( ) const ;

	Font* CreateSimpleFont( const std::string& name ) ;

	int Rotation( ) const ;

private :
	void AppendNode( PageNode *child ) ;
	void IncChildCount( ) ;
	
	void UpdateCount( ) const ;

	PageNode* CreateChild( DictReader& dict ) ;

private :
	PageInfo				m_pinfo ;

	std::vector<PageNode*>	m_kids ;
	
	//! number of leaf node children under this tree
	mutable int		m_count ;
} ;

} // end of namespace

#endif
