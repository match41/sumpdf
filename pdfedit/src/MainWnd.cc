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

#include "ExceptionDlg.hh"
#include "PageView.hh"
#include "PropertiesDlg.hh"
#include "TextDlg.hh"
#include "InsertTextDlg.hh"

// Qt headers
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QPointF>
#include <QPushButton>
#include <QSizePolicy>
#include <QTransform>
#include <QtGlobal>
#include <QToolBar>
#include <QFontComboBox>

#include "TextEdit.hh"
#include "GlyphGroup.hh"
#include "PageLoader.hh"
#include "Util.hh"

// libpdfdoc headers
#include <libpdfdoc.hh>
#include <Doc.hh>
#include <font/Font.hh>
#include <page/Page.hh>
#include <util/Exception.hh>
#include <util/Rect.hh>
#include <util/Debug.hh>
#include <util/Util.hh>
#include <graphics/Text.hh>
#include <graphics/TextLine.hh>
#include <graphics/TextState.hh>

#include <boost/bind.hpp>

#include <algorithm>
#include <cassert>
#include <sstream>

namespace pdf {

MainWnd::MainWnd( QWidget *parent )
	: QMainWindow( parent )
	, m_doc( CreateDoc() )
	, m_view( new PageView( this ) )
	, m_tool_bar( addToolBar(tr("Main") ) )
	, m_zoom_box( new QComboBox( m_tool_bar ) )
	, m_label( new QLabel( tr(" page:    ") ) )
	, m_current_page( 0 )
	, m_insert_text(new QPushButton( tr("&Insert Text") ) )
	, m_insert_dlg(new InsertTextDlg(this) )
{
	setupUi( this ) ;
	setCentralWidget( m_view ) ;

	connect( m_action_about,	SIGNAL(triggered()), this, SLOT(OnAbout()));
	connect( m_action_prop,		SIGNAL(triggered()), this, SLOT(OnProperties()));
	connect( m_action_open,		SIGNAL(triggered()), this, SLOT(OnOpen()) );
	connect( m_action_save_as,	SIGNAL(triggered()), this, SLOT(OnSaveAs()) );
	connect( m_action_exit, 	SIGNAL(triggered()), qApp, SLOT(quit()) );
	connect( m_action_previous_pg,	SIGNAL(triggered()),	this, SLOT(OnPreviousPage()) );
	connect( m_action_next_pg, 	SIGNAL(triggered()),	this, SLOT(OnNextPage()) );
	connect( m_action_first_pg,	SIGNAL(triggered()),	this, SLOT(OnFirstPage()) );
	connect( m_action_last_pg, 	SIGNAL(triggered()),	this, SLOT(OnLastPage()) );
	connect( m_action_viewsrc, 	SIGNAL(triggered()),	this, SLOT(OnViewSource()) );

	m_tool_bar->addAction( m_action_open ) ;

	m_zoom_box->addItem( "60%", 0.60 ) ;
	m_zoom_box->addItem( "85%", 0.85 ) ;
	m_zoom_box->addItem( "100%", 1.0 ) ;
	m_zoom_box->addItem( "125%", 1.25 ) ;
	m_zoom_box->addItem( "150%", 1.5 ) ;
	m_zoom_box->addItem( "175%", 1.75 ) ;
	m_zoom_box->addItem( "200%", 2.0 ) ;
	m_zoom_box->addItem( "250%", 2.5 ) ;
	m_zoom_box->addItem( "300%", 3.0 ) ;
	m_zoom_box->setCurrentIndex(2);

	connect(
		m_zoom_box,
		SIGNAL(currentIndexChanged(int)),
		this,
		SLOT(OnToolZoom(int)) ) ;
	
	m_tool_bar->addWidget( m_zoom_box ) ;

	m_tool_bar->addWidget( m_label );
	m_action_next_pg->setEnabled( false ) ;
	m_action_previous_pg->setEnabled( false ) ;
	m_action_first_pg->setEnabled( false ) ;
	m_action_last_pg->setEnabled( false ) ;
	
	m_item_prop->verticalHeader()->hide() ;
	m_item_prop->horizontalHeader()->setStretchLastSection( true ) ;

	CreateTextInsertToolbar();	// insert text feature
	
	// setup the scene and view
	GoToPage( 0 ) ;
}

/**	destructor is for the auto_ptr	
*/
MainWnd::~MainWnd( )
{
}

void MainWnd::OnChanged( const QList<QRectF>& region )
{
	OnSelectionChanged( ) ;
}

void MainWnd::OnSelectionChanged( )
{
	QList<QGraphicsItem*> items = CurrentScene()->selectedItems() ;
	if ( !items.empty() )
	{
		GlyphGroup *text = qgraphicsitem_cast<GlyphGroup*>( items.front() ) ;
		m_item_prop->setModel( text ) ;
	}
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
		// try to read the new PDF file. will throw exception in case of
		// any errors.
		Doc *new_doc = CreateDoc( ) ;
		new_doc->Read( ToStr( file ) ) ;
		PDF_ASSERT( new_doc->PageCount() > 0 ) ;
		
		// now the new file is read successfully, we destroy the data from
		// previous file and load the new stuff
		
		// destroy all pages from previous document
		std::for_each( m_pages.begin(), m_pages.end(),
			boost::bind( DeletePtr(),
				boost::bind( &PageMap::value_type::second, _1 ) ) ) ;
		m_pages.clear() ;
	
		// load the new stuff
		m_doc.reset( new_doc ) ;
		GoToPage( 0 ) ;
	}
	catch ( Exception& e )
	{
		ExceptionDlg dlg( e, this ) ;
		dlg.exec() ;
	}
}

void MainWnd::GoToPage( std::size_t page )
{
	if ( page >= 0 && page < m_doc->PageCount() )
	{
		m_current_page = page ;
	
		// go to next page and display
		PageMap::iterator it = m_pages.find( m_current_page ) ;
		QGraphicsScene *scene = 0 ;
		if ( it != m_pages.end() )
			scene = it->second ;
		else
		{
			scene = new QGraphicsScene( QRectF( 0, 0, 595, 842 ), this ) ;
			
			Page *p = m_doc->GetPage( m_current_page ) ;
			
			PageLoader loader( scene ) ;
			p->VisitGraphics( &loader ) ;

			connect(
				scene, 
				SIGNAL( selectionChanged() ),
				this,
				SLOT( OnSelectionChanged() ) ) ;
			m_pages.insert( std::make_pair( m_current_page, scene ) ) ;
		}
		
		scene->invalidate() ;
		m_view->setScene( scene ) ;
	
		m_label->setText( QString( tr(" page: %1 / %2") ).
			arg( m_current_page + 1 ).
			arg( m_doc->PageCount() ) ) ;
	
		// enable/disable page buttons
		m_action_next_pg->setEnabled( m_current_page + 1 < m_doc->PageCount() ) ;
		m_action_last_pg->setEnabled( m_current_page + 1 < m_doc->PageCount() ) ;
		m_action_previous_pg->setEnabled( m_current_page > 0 ) ;
		m_action_first_pg->setEnabled( m_current_page > 0 ) ;
	}
}

QGraphicsScene* MainWnd::CurrentScene()
{
	PageMap::iterator it = m_pages.find( m_current_page ) ;
	PDF_ASSERT( it != m_pages.end() ) ;
	PDF_ASSERT( it->second != 0 ) ;
	return it->second ;
}

void MainWnd::OnAbout( )
{
	std::ostringstream vs ;
	vs	<< "About PDF Editor\n"
		<< "version "    				<< VERSION		<< '\n'
		<< __DATE__ << '\n'
		<< "Using libpdfdoc version "	<< Version()	<< '\n'
		<< "Qt version "				<< qVersion()	<< '\n'
		<< "Copyright 2010 Nestal Wan\n"
		<< "License: GNU Public License Version 2" ;
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
		for ( PageMap::const_iterator i = m_pages.begin() ;
			i != m_pages.end() ; ++i )
		{
			Page *p = m_doc->GetPage( i->first ) ;
			StorePage( i->second, m_doc.get(), p ) ;
		}
		
		m_doc->Write( ToStr( fname)  ) ;
	}
}

void MainWnd::StorePage( QGraphicsScene *scene, Doc *doc, Page *page )
{
	assert( scene != 0 ) ;
	assert( doc != 0 ) ;
	assert( page != 0 ) ;
	
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

void MainWnd::OnNextPage( )
{
	if ( m_doc.get() )
	{
		try
		{
			if ( m_current_page < m_doc->PageCount()-1 )
			{
				// go to next page and display
				GoToPage( m_current_page + 1 ) ;
			}
		}
		catch ( Exception& e )
		{
			ExceptionDlg dlg( e, this ) ;
			dlg.exec() ;
		}

	}
}

void MainWnd::OnPreviousPage( )
{
	if ( m_doc.get() )
	{
		try
		{
			if ( m_current_page > 0 )
			{
				// go to previous page and display
				GoToPage( m_current_page - 1 ) ;
			}
		}
		catch ( Exception& e )
		{
			ExceptionDlg dlg( e, this ) ;
			dlg.exec() ;
		}
	}
}

void MainWnd::OnFirstPage( )
{
	if ( m_doc.get() )
	{
		try
		{
			// go to first page and display
			GoToPage( 0 ) ;
		}
		catch ( Exception& e )
		{
			ExceptionDlg dlg( e, this ) ;
			dlg.exec() ;
		}
	}
}

void MainWnd::OnLastPage( )
{
	if ( m_doc.get() )
	{
		try
		{
			// go to last page and display
			GoToPage( m_doc->PageCount() - 1 ) ;
		}
		catch ( Exception& e )
		{
			ExceptionDlg dlg( e, this ) ;
			dlg.exec() ;
		}
	}
}

void MainWnd::OnViewSource( )
{
	if ( m_doc.get() != 0 )
	{
		Page *p = m_doc->GetPage( m_current_page ) ;
		if ( p != 0 )
		{
			std::vector<unsigned char> c ;
			p->GetRawContent( c ) ;
			c.push_back( '\0' ) ;
			
			// size() must be > 0 after push_back()
			TextDlg dlg( reinterpret_cast<char*>(&c[0]), this ) ;
			dlg.exec() ;
		}
	}
}

void MainWnd::OnInsertDlg( )
{
	m_insert_dlg->show();
}

void MainWnd::CreateTextInsertToolbar( )
{
	m_toolbar_text->addWidget( m_insert_text );	// insert text push btn
	m_insert_text->setCheckable( true );

	m_insert_text_font = new QFontComboBox();	// font combo box
    m_insert_text_font_size = new QComboBox();	// font size combo box

    m_insert_text_font_size->setEditable(true);
    for (int i = 8; i < 30; i += 2)
        m_insert_text_font_size->addItem( QString().setNum( i ) );
    QIntValidator *validator = new QIntValidator( 2, 64, this );
    m_insert_text_font_size->setValidator( validator );

	m_toolbar_text->addWidget( m_insert_text_font );
	m_toolbar_text->addWidget( m_insert_text_font_size );


	connect( m_insert_text,		SIGNAL( clicked() ),	this, SLOT(OnInsertDlg() ) );

	connect(	// connect font selection between MainWnd and dialog (both ways)
		m_insert_text_font,
		SIGNAL( currentFontChanged( QFont ) ),
		m_insert_dlg,
		SLOT( SetFontChanged( QFont ) ) );
	connect(
		m_insert_dlg,
		SIGNAL( FontPropertiesChanged( QFont ) ),
		m_insert_text_font, 
		SLOT( setCurrentFont( QFont ) ) );

	connect(	// connect size selection between MainWnd and dialog (both ways)
		m_insert_text_font_size, 
		SIGNAL( currentIndexChanged( int ) ),
		m_insert_dlg, 
		SLOT( SetFontChanged( int ) ) );
	connect(
		m_insert_dlg, 
		SIGNAL( FontPropertiesChanged( int ) ),
		m_insert_text_font_size, 
		SLOT( setCurrentIndex( int ) ) );

	connect( 	// mouse position -> dlg
		m_view, 
		SIGNAL( mousePositionSet( QPointF ) ), 
		m_insert_dlg, 
		SLOT( OnMousePositionSet( QPointF ) ) );

	connect(	// Insert text into current scene
		m_insert_dlg, 
		SIGNAL( OnInsertClicked( ) ),	
		this, 
		SLOT( OnInsertTextNow( ) ) );
	connect(	// close Insert Dialog
		m_insert_dlg, 
		SIGNAL( OnDlgClosed( ) ),	
		this, 
		SLOT( OnInsertBtnUp( ) ) );

}

void MainWnd::OnInsertTextNow( )
{
	QTextEdit *text=m_insert_dlg->GetText( );
	QPointF pos=m_insert_dlg->GetPosition( );

	try
	{
		PDF_ASSERT( m_doc.get() != 0 ) ;

		QFont font = text->currentFont() ;
		Font *f = m_doc->CreateSimpleFont( ToStr( font.family() ) ) ;
		PDF_ASSERT( f != 0 ) ;

		TextState ts ;
		ts.SetFont( m_insert_dlg->GetFontSize().toInt(), f ) ;

		TextLine line( GraphicsState(ts),
			Matrix::Translation( pos.x(), pos.y() ), 
			ToWStr( text->toPlainText() ) ) ;

		CurrentScene()->addItem( new GlyphGroup( line ) ) ;
	}
	catch ( Exception& e )
	{
		ExceptionDlg dlg( e, this ) ;
		dlg.exec() ;
	}
}

void MainWnd::OnInsertBtnUp( )
{
	m_insert_text->setChecked( false );
}
} // end of namespace
