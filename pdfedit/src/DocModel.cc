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

/**	\file	DocModel.cc
	\brief	implementation of the DocModel class
	\date	Mar 27, 2010
	\author	Nestal Wan
*/

#include "DocModel.hh"

#include "GlyphGroup.hh"
#include "PageLoader.hh"
#include "Util.hh"

// libpdfdoc headers
#include <Doc.hh>
#include <libpdfdoc.hh>
#include <graphics/GraphicsState.hh>
#include <graphics/Text.hh>
#include <page/Page.hh>
#include <util/Debug.hh>
#include <util/Util.hh>

// Qt headers
#include <QGraphicsScene>
#include <QString>

// stdc++ headers
#include <algorithm>

namespace pdf {

/**	constructor
	
*/
DocModel::DocModel( QObject *parent )
	: QObject( parent )
	, m_doc( CreateDoc() )
	, m_current_page( 0 )
{
	GoToPage( 0 ) ;
}

void DocModel::OpenFile( const QString& filename )
{
	// try to read the new PDF file. will throw exception in case of
	// any errors.
	std::auto_ptr<Doc> new_doc( CreateDoc( ) ) ;
	new_doc->Read( ToStr( filename ) ) ;
	PDF_ASSERT( new_doc->PageCount() > 0 ) ;
	
	// now the new file is read successfully, we destroy the data from
	// previous file and load the new stuff
	ReplaceDocument( new_doc.release() ) ;
}

void DocModel::New( )
{
	// replace with a brand new document
	ReplaceDocument( CreateDoc() ) ;
}

void DocModel::ReplaceDocument( Doc *doc )
{
	// destroy all pages from previous document
	std::for_each( m_pages.begin(), m_pages.end(), DeletePtr() ) ;
	m_pages.clear() ;
	
	// load the new stuff
	m_doc.reset( doc ) ;
	m_pages.resize( m_doc->PageCount() ) ;
	GoToPage( 0 ) ;
}

QGraphicsScene* DocModel::CurrentScene()
{
	PDF_ASSERT( m_current_page < m_pages.size() ) ;
	PDF_ASSERT( m_pages[m_current_page] != 0 ) ;

	return m_pages[m_current_page] ;
}

std::size_t DocModel::CurrentPage( ) const
{
	return m_current_page ;
}

QGraphicsScene* DocModel::GoToPage( std::size_t page )
{
	PDF_ASSERT( m_current_page < m_pages.size() ) ;
	PDF_ASSERT( m_doc->PageCount() == m_pages.size() ) ;
	PDF_ASSERT( m_doc.get() != 0 ) ;
	
	// check array out of bound
	if ( page >= m_doc->PageCount() )
		return 0 ;
	
	// no change in page
	if ( m_current_page == page )
		return 0 ;
		
	m_current_page = page ;

	QGraphicsScene *scene = 0 ;
	if ( m_pages[m_current_page] != 0 )
		scene = m_pages[m_current_page] ;
	else
	{
		scene = new QGraphicsScene( QRectF( 0, 0, 595, 842 ), this ) ;
		connect(
			scene, 
			SIGNAL( selectionChanged() ),
			this,
			SLOT( OnSelectionChanged() ) ) ;

		Page *p = m_doc->GetPage( m_current_page ) ;
		PDF_ASSERT( p != 0 ) ;
		
		PageLoader loader( scene ) ;
		p->VisitGraphics( &loader ) ;
		
		m_pages[m_current_page] = scene ;
	}
	
	PDF_ASSERT( scene != 0 ) ;
	
	scene->invalidate() ;
	return scene ;
}

Doc* DocModel::Document( )
{
	PDF_ASSERT( m_doc.get() != 0 ) ;
	return m_doc.get() ;
}

void DocModel::OnSelectionChanged()
{
	emit SelectionChanged() ;
}

void DocModel::SaveFile( const QString& filename )
{
	for ( std::size_t i = 0 ; i < m_pages.size() ; ++i )
	{
		Page *p = m_doc->GetPage( i ) ;
		PDF_ASSERT( p != 0 ) ;
		
		StorePage( m_pages[i], p ) ;
	}
	
	m_doc->Write( ToStr( filename )  ) ;
}

void DocModel::StorePage( QGraphicsScene *scene, Page *page )
{
	PDF_ASSERT( scene != 0 ) ;
	PDF_ASSERT( page != 0 ) ;
	
	Text *t = CreateText( GraphicsState() ) ;
	
	QList<QGraphicsItem *> items = scene->items() ;
	for ( QList<QGraphicsItem*>::iterator i  = items.begin() ;
	                                      i != items.end() ; ++i )
	{
		GlyphGroup *text = qgraphicsitem_cast<GlyphGroup*>( *i ) ;
		
		if ( text != 0 )
		{
			PDF_ASSERT( text->Format().GetFont() != 0 ) ;
			t->AddLine( text->GetLine() ) ;
		}
	}
	
	std::vector<Graphics*> gfx( 1, t ) ;
	page->SetContent( gfx ) ;
	
}

} // end of namespace
