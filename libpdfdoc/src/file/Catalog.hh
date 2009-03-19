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
	\file	Catalog.hh
	\brief	definition the Catalog class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#ifndef __PDF_CATALOG_HEADER_INCLUDED__
#define __PDF_CATALOG_HEADER_INCLUDED__

#include "IElement.hh"

#include "core/Dictionary.hh"

#include <vector>

namespace pdf {

class Ref ;
class RealPage ;
class PageTree ;
class ElementList ;

/*!	\brief	brief description
	
	this class represents the document catalog of a PDF file
*/
class Catalog : public IElement
{
public :
	Catalog( ) ;

	void Read( const Ref& link, IElementSrc *repo ) ;
	void Write( const Ref& link, IElementDest *dest ) const ;

	void AddPage( RealPage *page ) ;

	std::size_t PageCount( ) const ;
	
	ElementList GetChildren( ) const ;
	
	RealPage*	GetPage( std::size_t index ) ;

private :
	Dictionary				m_self ;
	
	PageTree				*m_tree ;
} ;

} // end of namespace

#endif
