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

#include "TextObject.hh"
#include "PageLoader.hh"
#include "Util.hh"

// libpdfdoc headers
#include <Doc.hh>
#include <libpdfdoc.hh>
#include <font/Font.hh>
#include <graphics/GraphicsState.hh>
#include <graphics/TextLine.hh>
#include <graphics/TextState.hh>
#include <graphics/Text.hh>
#include <page/Page.hh>
#include <util/Debug.hh>
#include <util/Util.hh>
#include <util/Matrix.hh>
#include <util/Rect.hh>
#include <graphics/Color.hh>

#include <QDebug>

// Qt headers
#include <QGraphicsRectItem>
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
	, m_pages( 1, static_cast<QGraphicsScene*>(0) )
	, m_current_page( 0 )
{
	GoToPage( 0 ) ;
}

void DocModel::SetCurrentFile( const QString& file )
{
	if ( m_current_file != file )
	{
		m_current_file = file;
		emit CurrentFileChanged();
	}
}

QString DocModel::GetCurrentFile( ) const
{
	return m_current_file;
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
	SetCurrentFile( filename );
}

void DocModel::New( )
{
	// replace with a brand new document
	ReplaceDocument( CreateDoc() ) ;
	SetCurrentFile( tr("Untitled") ) ;
}

void DocModel::ReplaceDocument( Doc *doc )
{
	// destroy all pages from previous document
	std::for_each( m_pages.begin(), m_pages.end(), DeletePtr() ) ;
	m_pages.clear() ;
	
	// load the new stuff
	m_doc.reset( doc ) ;
	m_pages.resize( m_doc->PageCount() ) ;
	m_current_page = 0 ;
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

std::size_t DocModel::PageCount( ) const
{
	return m_doc->PageCount() ;
}

Page* DocModel::GetPage( std::size_t idx )
{
	return m_doc->GetPage( idx ) ;
}

QGraphicsScene* DocModel::GoToPage( std::size_t page )
{
	PDF_ASSERT( page < m_doc->PageCount() ) ;
	PDF_ASSERT( m_current_page < m_pages.size() ) ;
	PDF_ASSERT( m_doc->PageCount() == m_pages.size() ) ;
	PDF_ASSERT( m_doc.get() != 0 ) ;
	
	m_current_page = page ;
	
	QGraphicsScene *scene = 0 ;
	if ( m_pages[m_current_page] != 0 )
		scene = m_pages[m_current_page] ;
	else
	{
		// create a new scene and set the background as gray
		scene = new QGraphicsScene( QRectF( 0, 0, 595, 842 ), this ) ;
		scene->setBackgroundBrush( Qt::gray ) ;
		
		connect(
			scene, 
			SIGNAL( selectionChanged() ),
			this,
			SLOT( OnSelectionChanged() ) ) ;

		Page *p = m_doc->GetPage( m_current_page ) ;
		PDF_ASSERT( p != 0 ) ;
		
		// add a white solid box to represent the page
		Rect rect = p->MediaBox() ;
		QGraphicsRectItem *rect_item = scene->addRect( ToQRectF( rect ) ) ;
		rect_item->setBrush( Qt::white ) ;

		PageLoader loader( scene ) ;
		p->VisitGraphics( &loader ) ;
		
		m_pages[m_current_page] = scene ;
	}
	
	// triggers redraw
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

		if ( m_pages[i] != 0 )
			StorePage( m_pages[i], p ) ;
	}
	
	m_doc->Write( ToStr( filename )  ) ;
	
	SetCurrentFile( filename );
}

void DocModel::StorePage( QGraphicsScene *scene, Page *page )
{
	PDF_ASSERT( scene != 0 ) ;
	PDF_ASSERT( page != 0 ) ;
	
	// need to traverse in ascending order
	std::vector<Graphics*> gfx ;
	QList<QGraphicsItem *> items = scene->items( Qt::AscendingOrder ) ;
	for ( QList<QGraphicsItem*>::iterator i  = items.begin() ;
	                                      i != items.end() ; ++i )
	{
		GraphicsObject *gobj = dynamic_cast<GraphicsObject*>( *i ) ;
		if ( gobj != 0 )
		{
			Graphics *g = gobj->Write( ) ;
			if ( g != 0 )
				gfx.push_back( g ) ;
		}
	}

	page->SetContent( gfx ) ;
	
	std::for_each( gfx.begin(), gfx.end(), DeletePtr() ) ;
}

void DocModel::AddText(
	const QFont&	font,
	double			size,
	const QPointF&	pos,
	const QString&	text, 
	const QColor	c )
{
	Font *f = m_doc->CreateSimpleFont( ToStr( font.family() ) ) ;
	PDF_ASSERT( f != 0 ) ;

	GraphicsState gs ;
	gs.Text().ChangeFont( size, f ) ;
	gs.FillColor(FromQColor(c));	// set text color

	TextLine line( gs,
		Matrix::Translation( pos.x(), pos.y() ), 
		ToWStr( text ) ) ;

	m_pages[m_current_page]->addItem( new TextObject( line ) ) ;

}

} // end of namespace
