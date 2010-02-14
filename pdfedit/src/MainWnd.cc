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

/**
	\file	MainWnd.cc
	\brief	definition the MainWnd class
	\date	Dec 27, 2009
	\author	Nestal Wan
*/

#include "MainWnd.hh"

#include "PageView.hh"
#include "PropertiesDlg.hh"
#include "ExceptionDlg.hh"

// Qt headers
#include <QApplication>
#include <QComboBox>
#include <QFileDialog>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QFont>
#include <QFontDialog>
#include <QList>
#include <QMessageBox>
#include <QPointF>
#include <QTransform>
#include <QToolBar>

#include <QGraphicsRectItem>

#include <QDebug>

#include "TextEdit.hh"
#include "GlyphGroup.hh"
#include "Util.hh"

// libpdfdoc headers
#include <libpdfdoc.hh>
#include <Doc.hh>
#include <font/Font.hh>
#include <page/Page.hh>
#include <util/Rect.hh>
#include <util/Debug.hh>
#include <page/PageContent.hh>
#include <graphics/Text.hh>
#include <graphics/TextLine.hh>

#include <boost/bind.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

namespace pdf {

MainWnd::MainWnd( QWidget *parent )
	: QMainWindow( parent ),
	  m_scene( new QGraphicsScene( QRectF( 0, 0, 595, 842 ), this ) ),
	  m_view( new PageView( m_scene, this ) ),
	  m_tool_bar( addToolBar(tr("Main") ) ),
	  m_zoom_box( new QComboBox( m_tool_bar ) )
{
	setupUi( this ) ;
	setCentralWidget( m_view ) ;
	
	connect( m_action_about,	SIGNAL(triggered()), this, SLOT(OnAbout()));
	connect( m_action_prop,		SIGNAL(triggered()), this, SLOT(OnProperties()));
	connect( m_action_open,		SIGNAL(triggered()), this, SLOT(OnOpen()) );
	connect( m_action_save_as,	SIGNAL(triggered()), this, SLOT(OnSaveAs()) );
	connect( m_action_font, 	SIGNAL(triggered()), this, SLOT(OnEditFont()) );
	connect( m_action_exit, 	SIGNAL(triggered()), qApp, SLOT(quit()) );

	// initialize tool bar
	m_tool_bar->addAction( m_action_open ) ;
	
	m_zoom_box->addItem( "100%", 1.0 ) ;
	m_zoom_box->addItem( "125%", 1.25 ) ;
	m_zoom_box->addItem( "150%", 1.5 ) ;
	m_zoom_box->addItem( "175%", 1.75 ) ;
	m_zoom_box->addItem( "200%", 2.0 ) ;
	m_zoom_box->addItem( "250%", 2.5 ) ;
	m_zoom_box->addItem( "300%", 3.0 ) ;
	connect(
		m_zoom_box,
		SIGNAL(currentIndexChanged(int)),
		this,
		SLOT(OnToolZoom(int)) ) ;
	
	m_tool_bar->addWidget( m_zoom_box ) ;
	
	QGraphicsRectItem *item = new QGraphicsRectItem( 100, 100, m_view->physicalDpiX(), m_view->physicalDpiX() ) ;
	qDebug() << m_view->physicalDpiX() << " " << m_view->physicalDpiY() ;
	m_scene->addItem( item ) ;
}

/**	destructor is for the auto_ptr	
*/
MainWnd::~MainWnd( )
{
}

void MainWnd::OnEditFont( )
{
/*	bool ok = true ;
	QFont f = QFontDialog::getFont( &ok, this ) ;
	if ( ok )
	{
		FT_Face face = f.freetypeFace() ;

		FcChar8 abc[] = "abc", *f2 ;

		FcBlanks *b = FcBlanksCreate() ;
		FcPattern *p = FcFreeTypeQueryFace( face, abc, 0, b ) ;
		if (FcPatternGetString( p, FC_FILE, 0, &f2) == FcResultMatch)
			std::cout << "oops: " << f2 << std::endl ;

		FcChar8 *filename2 ;
		FcResult result ;
		FcPattern *matched = FcFontMatch( 0, p, &result);
		
		if (FcPatternGetString (matched, FC_FILE, 0, &filename2) != FcResultMatch)
			std::cout << "oops" << std::endl ;

		int id ;
		if (FcPatternGetInteger (matched, FC_INDEX, 0, &id) != FcResultMatch)
			std::cout << "oops2" << std::endl ;
		  
		std::cout << "file is " << filename2 << " " << id << std::endl ; 

	}*/
}

void MainWnd::OnToolZoom( int choice )
{
	PDF_ASSERT( m_zoom_box != 0 ) ;

	qDebug() << "zoom to " << m_zoom_box->itemData( choice ).toDouble() ;
	m_view->Zoom( m_zoom_box->itemData( choice ).toDouble() ) ;
}

void MainWnd::OpenFile( const QString& file )
{
	try
	{
		m_doc.reset( CreateDoc( ) ) ;
		m_doc->Read( file.toStdString() ) ;
		
		// it should have thrown exception when error.
		// can now clear the screen.
		m_scene->clear( ) ;
		
		if ( m_doc->PageCount() > 0 )
		{
			// only load page 0 for now
			Page *p = m_doc->GetPage( 0 ) ;
			Rect r = p->MediaBox( ) ;
			m_scene->setSceneRect( r.Left(), r.Bottom(), r.Width(), r.Height());
			
			PageContent *c = p->GetContent( ) ;
			c->VisitGraphics( this ) ;
		}
	}
	catch ( std::exception& e )
	{
		ExceptionDlg dlg( e.what(), this ) ;
		dlg.exec() ;
	}
}

void MainWnd::VisitText( Text *text )
{
	assert( text != 0 ) ;

	std::for_each( text->begin(), text->end(),
		boost::bind( &MainWnd::LoadTextLine, this, _1 ) ) ;
}

void MainWnd::LoadTextLine( const TextLine& line )
{
	GlyphGroup *group = new GlyphGroup( line ) ;
	group->setTransform( ToQtMatrix( line.Transform() ) ) ;
	m_scene->addItem( group ) ;
}

void MainWnd::VisitGraphics( Graphics *gfx )
{
}

void MainWnd::OnAbout( )
{
	std::ostringstream vs ;
	vs	<< "PDF Editor version " << Version() << '\n'
		<< "Copyright Nestal Wan 2010\n"
		<< "License: GPL" ;
	QMessageBox::information( this, "About PDF Editor", vs.str().c_str() ) ;
}

void MainWnd::OnProperties( )
{
	if ( m_doc.get() )
	{
		PropertiesDlg dlg( m_doc.get(), this ) ;
		dlg.exec( ) ;
	}
}

void MainWnd::OnOpen( )
{
	QString fname = QFileDialog::getOpenFileName( this, "Open", ".", "*.pdf" ) ;
	if ( !fname.isEmpty( ) )
		OpenFile( fname ) ;
}

void MainWnd::OnSaveAs( )
{
	QString fname = QFileDialog::getSaveFileName( this, "Open", ".", "*.pdf" ) ;
	if ( !fname.isEmpty( ) )
	{
		// we shouldn't do like this. we shouldn't create another doc.
		// we should re-use m_doc.
		Page *p = m_doc->GetPage( 0 ) ;
		StorePage( m_scene, m_doc.get(), p ) ;
		
		m_doc->Write( fname.toStdString() ) ;
	}
}

void MainWnd::StorePage( QGraphicsScene *scene, Doc *doc, Page *page )
{
	assert( scene != 0 ) ;
	assert( doc != 0 ) ;
	assert( page != 0 ) ;
	
	PageContent *c = page->GetContent( ) ;
	c->Clear( ) ;
	
	Text *t = c->AddText( TextState() ) ;
	
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
}

} // end of namespace
