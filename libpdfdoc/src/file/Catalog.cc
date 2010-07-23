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
	\file	Catalog.cc
	\brief	implementation the Catalog class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#include "Catalog.hh"

#include "File.hh"
#include "NameTree.hh"
#include "DictReader.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"

#include "font/BaseFont.hh"
#include "font/FontSubsetInfo.hh"
#include "graphics/GraphicsVisitor.hh"
#include "graphics/Text.hh"
#include "graphics/RealTextLine.hh"
#include "graphics/TextState.hh"

#include "page/RealPage.hh"
#include "page/PageTree.hh"

#include "util/Debug.hh"
#include "util/Exception.hh"
#include "util/Util.hh"

#include <boost/format.hpp>

#include <set>
#include <iostream>
#include <iterator>

namespace pdf {

struct Catalog::NameDict
{
	NameTree	m_dests, m_ap, m_javascript, m_pages, m_temps, m_ids, m_urls,
				m_emb_files, m_alt_present, m_rend ;

	void Read( Dictionary& self, File *file )
	{
//		DictReader reader( self, file ), dests ;
//		
//		if ( reader.Detach( "Dests", dests ) )
//			m_dests.Read( dests, file ) ;
	}
} ;

class Catalog::FontSubset : public FontSubsetInfo, public GraphicsVisitor
{
public :
	FontSubset( ) ;

	std::vector<wchar_t> GetUsedChars( const BaseFont *f ) const ;

	void VisitText( Text *text ) ;
	void VisitGraphics( Graphics *text ) ;
	void VisitPath( Path *path ) ;
	void VisitGraphicsLink( ExtGraphicsLink<Image> *img ) ;
	void VisitGraphicsLink( ExtGraphicsLink<GraphicsGroup> *g ) ;
	
private :
	typedef std::set<wchar_t> CharSet ;
	typedef std::map<const BaseFont*, CharSet> FontChars ;
	
	FontChars	m_used_chars ;
} ;

Catalog::Catalog( FontDb *fontdb )
	: m_version		( "1.4" )
	, m_page_layout	( "SinglePage" )
	, m_page_mode	( "UseNode" )
	, m_tree		( new PageTree( fontdb ) )
	, m_name_dict	( new NameDict )
{
}

Catalog::Catalog( const Ref& link, File *file, FontDb *fontdb )
	: m_version		( "1.4" )
	, m_page_layout	( "SinglePage" )
	, m_page_mode	( "UseNode" )
	, m_tree		( 0 )
	, m_name_dict	( new NameDict )
{
	PDF_ASSERT( file != 0 ) ;
	Dictionary d = file->ReadObj( link ).As<Dictionary>() ;

	DictReader self( d, file ) ;

	Name type ;
	if ( !self.Detach( "Type", type ) || type != "Catalog" )
	{
		using boost::format ;
		throw ParseError()
			<< expt::FormattedMsg( format("invalid catalog type: %1%") % type ) ;
	}

	// page tree is mandatory
	DictReader tree ;
	if ( !self.Detach( "Pages", tree ) )
		throw ParseError() << expt::ErrMsg( "no page tree in catalog" ) ;
	
	// root page tree has no parent
	m_tree = new PageTree( fontdb ) ;
	m_tree->Read( tree ) ;
	
	self.Detach( "Version",		m_version ) ;
	self.Detach( "PageLayout",	m_page_layout ) ;
	self.Detach( "PageMode",	m_page_mode ) ;

	// read destintions
	DictReader dest ;
	if ( self.Detach( "Dests", dest ) )
	{
		for ( Dictionary::iterator i = dest->begin() ; i != dest->end() ; ++i )
		{
			Array darray ;
			self.SwapAt( i, darray ) ; 
			
			Destination d ;
			d.Read( darray, self.GetFile() ) ;
			
			m_named_dests.insert( std::make_pair( i->first, d ) ) ;
		}
	}

	Dictionary name_dict ;
	if ( self.Detach( "Names", name_dict ) )
		m_name_dict->Read( name_dict, file ) ;
}

Catalog::~Catalog( )
{
	// clear the destination map first. it should free up all links to
	// the pages.
	m_named_dests.clear( ) ;

	// there should be no one linking to the root tree node now.
	PDF_ASSERT( m_tree->UseCount() == 1 ) ;
	
	m_tree->Release() ;
	m_tree = 0 ;
}

Ref Catalog::Write( File *file ) const
{
	Dictionary self ;

	FontSubset subset ;
	for ( std::size_t i = 0 ; i < PageCount() ; i++ )
	{
		const Page *page = GetPage( i ) ;
		PDF_ASSERT( page != 0 ) ;
		page->VisitGraphics( &subset ) ;
	}

	Ref tree = file->AllocLink( ) ;
	m_tree->Write( tree, file, Ref(), &subset ) ; 

	self.insert( "Pages",  	tree ) ;
	self.insert( "Type",	Name( "Catalog" ) ) ;
	
	self.insert( "Version",		m_version ) ;
	self.insert( "PageLayout",	m_page_layout ) ;
	self.insert( "PageMode",	m_page_mode ) ;

	// write destinations
	Dictionary dest ;
	for ( std::map<Name, Destination>::const_iterator i = m_named_dests.begin() ;
		i != m_named_dests.end() ; ++i )
	{
		dest.insert( std::make_pair( i->first, i->second.Write( file ) ) ) ;
	}
	if ( !dest.empty() )
		self.insert( "Dests", file->WriteObj(dest) ) ;

	return file->WriteObj( self ) ;
}

Page* Catalog::AddPage( )
{
	PDF_ASSERT( m_tree != 0 ) ;

	// for now the pages are arranged linearly for now
	return new RealPage( m_tree ) ;
}

std::size_t Catalog::PageCount( ) const
{
	PDF_ASSERT( m_tree != 0 ) ;

	return m_tree->Count( ) ;
}

Page* Catalog::GetPage( std::size_t index )
{
	PDF_ASSERT( m_tree != 0 ) ;

	PageNode *p = m_tree->GetLeaf( index ) ;
	PDF_ASSERT( p != 0 ) ;
	
	// no need to use dynamic cast as it will not be child class of RealPage
	PDF_ASSERT( typeid(*p) == typeid(RealPage) ) ;

	return static_cast<RealPage*>( p ) ;
}

const Page* Catalog::GetPage( std::size_t index ) const
{
	PDF_ASSERT( m_tree != 0 ) ;

	PageNode *p = m_tree->GetLeaf( index ) ;
	PDF_ASSERT( p != 0 ) ;
	
	// no need to use dynamic cast as it will not be child class of RealPage
	PDF_ASSERT( typeid(*p) == typeid(RealPage) ) ;

	return static_cast<RealPage*>( p ) ;
}

Catalog::FontSubset::FontSubset( )
{
}

std::vector<wchar_t> Catalog::FontSubset::GetUsedChars(const BaseFont *f) const
{
	FontChars::const_iterator i = m_used_chars.find( f ) ;
	return i != m_used_chars.end()
		? std::vector<wchar_t>( i->second.begin(), i->second.end() )
		: std::vector<wchar_t>( ) ;
}

void Catalog::FontSubset::VisitText( Text *text )
{
//	for ( Text::iterator i = text->begin() ; i != text->end() ; ++i )
	for ( std::size_t i = 0 ; i < text->Count() ; i++ )
	{
		TextLine *line = text->At( i ) ;
	
		const std::wstring& text = line->Text() ;
		const BaseFont* font = static_cast<BaseFont*>(
			line->Format().Text().FontFace() ) ;
		
		CharSet& cs = m_used_chars[font] ;
		cs.insert( text.begin(), text.end() ) ;
	}
}

void Catalog::FontSubset::VisitGraphics( Graphics *text )
{
}

void Catalog::FontSubset::VisitPath( Path *path )
{
}

void Catalog::FontSubset::VisitGraphicsLink( ExtGraphicsLink<Image> *img )
{
}

void Catalog::FontSubset::VisitGraphicsLink( ExtGraphicsLink<GraphicsGroup> *g )
{
}

} // end of namespace
