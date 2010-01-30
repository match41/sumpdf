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
	\file	RealPage.hh
	\brief	definition the RealPage class
	\date	Sat Mar 15 2008
	\author	Nestal Wan
*/

#ifndef __PDF_REAL_PAGE_HEADER_INCLUDED__
#define __PDF_REAL_PAGE_HEADER_INCLUDED__

// base class headers
#include "page/Page.hh"
#include "PageNode.hh"

// other libpdfdoc headers
#include "RealContent.hh"
#include "RealResources.hh"
#include "core/Dictionary.hh"
#include "core/Token.hh"
#include "file/CompleteObj.hh"
#include "stream/Stream.hh"
#include "util/Rect.hh"

#include <string>

namespace pdf {

class Dictionary ;
class Font ;
class Name ;
class Stream ;
class RealResources ;
class File ;
class Graphics ;

/*!	\brief	real implementation of a page
	
	This class represents a page in the PDF document. It is an implementation
	of the Page interface.
*/
class RealPage : public Page, public PageNode
{
public :
	explicit RealPage( PageTree *parent ) ;

	void Read( Dictionary& dict, File *file ) ;
	void Write( const Ref& link, File *file, const Ref& parent ) const ;
	
	Rect MediaBox( ) const ;

	void Clear( ) ;
	
	std::size_t Count( ) const ;
	PageTree* Parent( ) ;
	
	PageNode* GetLeaf( std::size_t index ) ;
	
	RealResources* GetResource( ) ;
	const RealResources* GetResource( ) const ;

	RealContent* GetContent( ) ;

private :
	void ReadContent( const Object& str_obj, File *file ) ;
	Object WriteContent( File *file ) const ; 

	void Decode( ) ;
	Graphics* ProcessCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t		count,
		Graphics		*gfx ) ;

private :
	PageTree		*m_parent ;
	RealResources	m_resources ;
//	CompleteObj		m_self ;
	
	/// in PDF user space. specified by UserUnit or 1/72 inch
	Rect		m_media_box ;
	
	RealContent	m_content ;

	std::vector<Stream>	m_cstrs ;

	int	m_rotate ;
} ;

} // end of namespace

#endif
