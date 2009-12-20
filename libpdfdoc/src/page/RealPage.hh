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

#include "PaintOp.hh"
#include "Resources.hh"

#include "core/Dictionary.hh"
#include "core/Token.hh"
#include "stream/Stream.hh"
#include "util/Rect.hh"

#include <string>

namespace pdf {

class Dictionary ;
class Font ;
class Name ;
class Stream ;
class Resources ;
class IFile ;

/*!	\brief	real implementation of a page
	
	This class represents a page in the PDF document. It is an implementation
	of the Page interface.
*/
class RealPage : public Page, public PageNode
{
public :
	RealPage( PageTree *parent, const Dictionary& link, IFile *file ) ;
	explicit RealPage( PageTree *parent ) ;

	void Write( const Ref& link, IFile *file, const Ref& parent ) const ;
	
	Rect MediaBox( ) const ;

	void DrawText( double x, double y, Font *font, const std::string& text ) ;

	std::size_t Count( ) const ;
	PageTree* Parent( ) ;
	
	PageNode* GetLeaf( std::size_t index ) ;
	
private :
	void ReadContent( const Object& str_obj, IFile *file ) ;
	void DecodeContent( const Stream& s ) ;
	Object WriteContent( IFile *file ) const ; 
	
private :
	PageTree	*m_parent ;
	Dictionary	m_self ;
	Rect		m_media_box ;
	
//	std::vector<PaintOp>	m_contents ;
	std::vector<Stream>		m_content ;

	int	m_rotate ;
} ;

} // end of namespace

#endif
