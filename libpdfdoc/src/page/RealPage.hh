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
	\file	RealPage.hh
	\brief	definition the RealPage class
	\date	Sat Mar 15 2008
	\author	Nestal Wan
*/

#ifndef __PDF_REAL_PAGE_HEADER_INCLUDED__
#define __PDF_REAL_PAGE_HEADER_INCLUDED__

#include "Page.hh"
#include "PageNode.hh"

#include "Resources.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"
#include "core/Token.hh"
#include "util/Rect.hh"

#include <string>

namespace pdf {

class Dictionary ;
class Font ;
class Name ;
class Stream ;
class Resources ;

/*!	\brief	real implementation of a page
	
	This class represents a page in the PDF document. It is an implementation
	of the Page interface.
*/
class RealPage : public Page, public PageNode
{
public :
	explicit RealPage( PageTree *parent = 0 ) ;

	void Init( Object& link, IElementSrc *repo ) ;
	void Write( const Ref& link, IElementDest *repo ) const ;
	
	Rect MediaBox( ) const ;

	void DrawText( double x, double y, Font *font, const std::string& text ) ;

	std::size_t Count( ) const ;
	
	PageNode* GetLeaf( std::size_t index ) ;
	
private :
	void ReadContent( const Object& str_obj, IElementSrc *src ) ;
	void DecodeContent( const Stream& s ) ;
	
private :
	Dictionary	m_self ;
	Rect		m_media_box ;
	
	std::vector<Token>	m_contents ;

	mutable enum Status { disk_newer, memory_newer, sync } m_status ;

	int	m_rotate ;
} ;

} // end of namespace

#endif
