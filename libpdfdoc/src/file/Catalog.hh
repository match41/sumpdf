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
	\file	Catalog.hh
	\brief	definition the Catalog class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#ifndef __PDF_CATALOG_HEADER_INCLUDED__
#define __PDF_CATALOG_HEADER_INCLUDED__

#include "core/Name.hh"
#include "file/Destination.hh"

#include <map>
#include <memory>

struct FT_LibraryRec_ ;

namespace pdf {

class ElementList ;
class File ;
class Font ;
class FontDb ;
class PageTree ;
class Ref ;
class Page ;

/*!	\brief	brief description
	\internal

	This class represents the document catalog of a PDF file. The document
	catalog stores the main page tree of the document.
*/
class Catalog
{
public :
	explicit Catalog( FontDb *fontdb ) ;
	Catalog( const Ref& link, File *file, FontDb *fontdb ) ;
	~Catalog( ) ;
	
	Ref Write( File *file ) const ;

	Page* AddPage( ) ;

	std::size_t PageCount( ) const ;

	Page*	GetPage( std::size_t index ) ;
	const Page* GetPage( std::size_t index ) const ;

private :
	Name		m_version ;
	Name		m_page_layout ;
	Name		m_page_mode ;

	PageTree	*m_tree ;
	std::map<Name, Destination>	m_named_dests ;
	
	// name dictionary
	struct NameDict ;
	std::auto_ptr<NameDict>	m_name_dict ;
	
	class FontSubset ;
} ;

} // end of namespace

#endif
