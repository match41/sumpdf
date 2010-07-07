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

// class header
#include "MainWnd.hh"

#include "DocModel.hh"
#include "PageView.hh"
#include "Util.hh"

// dialogs
#include "dialog/ExceptionDlg.hh"
#include "dialog/PropertiesDlg.hh"
#include "dialog/TextDlg.hh"

#include "graphics/GraphicsDelegate.hh"
#include "graphics/GraphicsObject.hh"

// libpdfdoc headers
#include <libpdfdoc.hh>
#include <Doc.hh>
#include <page/Page.hh>
#include <util/Exception.hh>
#include <util/Rect.hh>
#include <util/Debug.hh>
#include <util/Util.hh>

// Qt headers
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QPointF>
#include <QSizePolicy>
#include <QTransform>
#include <QtGlobal>
#include <QToolBar>

// boost headers
#include <boost/bind.hpp>

// stdc++ headers
#include <algorithm>
#include <cassert>
#include <sstream>

namespace pdf {

MainWnd::MainWnd( QWidget *parent )
	: QMainWindow( parent )
	, m_doc( new DocModel( this ) )
	, m_view( new PageView( this, m_doc ) )
	, m_tool_bar( addToolBar(tr("Main") ) )
	, m_zoom_box( new QComboBox( m_tool_bar ) )
	, m_label( new QLabel( tr(" page:    ") ) )
	, m_delegate( new GraphicsDelegate( this ) )
{
	setupUi( this ) ;
	setCentralWidget( m_view ) ;
	m_view->SetStatusBar( m_status_bar ) ;

	connect( m_action_about,	SIGNAL(triggered()), this, SLOT(OnAbout()));
	connect( m_action_prop,		SIGNAL(triggered()), this, SLOT(OnProperties()));
	connect( m_action_open,		SIGNAL(triggered()), this, SLOT(OnOpen()) );
	connect( m_action_new,		SIGNAL(triggered()), this, SLOT(OnNew()) );
	connect( m_action_save_as,	SIGNAL(triggered()), this, SLOT(OnSaveAs()) );
	connect( m_action_exit, 	SIGNAL(triggered()), qApp, SLOT(quit()) );
	connect( m_action_previous_pg,	SIGNAL(triggered()),	this, SLOT(OnPreviousPage()) );
	connect( m_action_next_pg, 	SIGNAL(triggered()),	this, SLOT(OnNextPage()) );
	connect( m_action_first_pg,	SIGNAL(triggered()),	this, SLOT(OnFirstPage()) );
	connect( m_action_last_pg, 	SIGNAL(triggered()),	this, SLOT(OnLastPage()) );
	connect( m_action_viewsrc, 	SIGNAL(triggered()),	this, SLOT(OnViewSource()) );
	connect( m_action_append, 	SIGNAL(triggered()), 	this, SLOT(OnAppendPage()) );

	connect( m_doc, SIGNAL( SelectionChanged() ),
			 this, SLOT( OnSelectionChanged() ) );
	connect( m_doc, SIGNAL( CurrentFileChanged() ),
			 this, SLOT( OnFileChanged() ) );

	OnFileChanged( );

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
	
	// toolbar initialization
	m_tool_bar->addWidget( m_zoom_box ) ;
	m_tool_bar->addWidget( m_label );
	m_action_next_pg->setEnabled( false ) ;
	m_action_previous_pg->setEnabled( false ) ;
	m_action_first_pg->setEnabled( false ) ;
	m_action_last_pg->setEnabled( false ) ;
	
	// properties window initialization
	m_item_prop->verticalHeader()->hide() ;
	m_item_prop->horizontalHeader()->setStretchLastSection( true ) ;
	m_item_prop->setItemDelegate( m_delegate ) ;

	// text insert
	QActionGroup *g = new QActionGroup( this ) ;
	g->addAction( m_action_pointer_tool ) ;
	g->addAction( m_action_text_tool ) ;
	g->addAction( m_action_zoom_tool ) ;
	ConnectToolbarSignals( ) ;
	
	// setup the scene and view
	GoToPage( 0 ) ;
}

/**	destructor is for the auto_ptr	
*/
MainWnd::~MainWnd( )
{
}

void MainWnd::OnNew( )
{
	m_view->setScene( 0 ) ;
	m_doc->New( ) ;
	m_view->setScene( m_doc->CurrentScene() ) ;
	GoToPage( 0 );
}

void MainWnd::OnChanged( const QList<QRectF>& region )
{
	OnSelectionChanged( ) ;
}

void MainWnd::OnSelectionChanged( )
{
	QList<QGraphicsItem*> items = m_doc->CurrentScene()->selectedItems() ;
	if ( !items.empty() )
	{
		GraphicsObject *obj = dynamic_cast<GraphicsObject*>( items.front() ) ;
		m_item_prop->setModel( obj ) ;
	}
}

void MainWnd::OnToolZoom( int choice )
{
	PDF_ASSERT( m_zoom_box != 0 ) ;

	m_view->Zoom( m_zoom_box->itemData( choice ).toDouble() ) ;
}

void MainWnd::OpenFile( const QString& file )
{
	try
	{
		m_doc->OpenFile( file ) ;
		GoToPage( 0 ) ;
	}
	catch ( Exception& e )
	{
		ExceptionDlg dlg( e, this ) ;
		dlg.exec() ;
	}
	catch ( std::exception& e )
	{
		ExceptionDlg dlg( e.what(), this ) ;
		dlg.exec() ;
	}
	catch ( ... )
	{
		ExceptionDlg dlg( "unknown exception", this ) ;
		dlg.exec() ;
	}
}

void MainWnd::GoToPage( std::size_t page )
{
	QGraphicsScene *scene = m_doc->GoToPage( page ) ;

	PDF_ASSERT( scene != 0 ) ;
	m_view->setScene( scene ) ;

	m_label->setText( QString( tr(" page: %1 / %2") ).
		arg( m_doc->CurrentPage() + 1 ).
		arg( m_doc->PageCount() ) ) ;

	// enable/disable page buttons
	m_action_next_pg->setEnabled( m_doc->CurrentPage() + 1 < m_doc->PageCount() ) ;
	m_action_last_pg->setEnabled( m_doc->CurrentPage() + 1 < m_doc->PageCount() ) ;
	m_action_previous_pg->setEnabled( m_doc->CurrentPage() > 0 ) ;
	m_action_first_pg->setEnabled( m_doc->CurrentPage() > 0 ) ;
}

void MainWnd::OnAbout( )
{
	std::ostringstream vs ;
	vs	<< "About SumPDF\n"
		<< "version "    				<< VERSION		<< '\n'
		<< __DATE__ << '\n'
		<< "Using libpdfdoc version "	<< Version()	<< '\n'
		<< "Qt version "				<< qVersion()	<< '\n'
		<< "Copyright 2010 Nestal Wan\n"
		<< "License: GNU Public License Version 2" ;
	QMessageBox::information( this, "About SumPDF", vs.str().c_str() ) ;
}

void MainWnd::OnProperties( )
{
	PropertiesDlg dlg( m_doc->Document(), this ) ;
	dlg.exec( ) ;
}

void MainWnd::OnOpen( )
{
	QString fname = QFileDialog::getOpenFileName( this, tr("Open"), ".", "*.pdf" ) ;
	if ( !fname.isEmpty( ) )
		OpenFile( fname ) ;
}

void MainWnd::OnSaveAs( )
{
	QString fname = QFileDialog::getSaveFileName( this, tr("Save As"),
		m_doc->GetCurrentFile(), "*.pdf" ) ;
	if ( !fname.isEmpty( ) )
	{
		m_doc->SaveFile( fname ) ;
	}
}

void MainWnd::OnNextPage( )
{
	try
	{
		if ( m_doc->CurrentPage() < m_doc->PageCount()-1 )
		{
			// go to next page and display
			GoToPage( m_doc->CurrentPage() + 1 ) ;
		}
	}
	catch ( Exception& e )
	{
		ExceptionDlg dlg( e, this ) ;
		dlg.exec() ;
	}
}

void MainWnd::OnPreviousPage( )
{
	try
	{
		if ( m_doc->CurrentPage() > 0 )
		{
			// go to previous page and display
			GoToPage( m_doc->CurrentPage() - 1 ) ;
		}
	}
	catch ( Exception& e )
	{
		ExceptionDlg dlg( e, this ) ;
		dlg.exec() ;
	}
}

void MainWnd::OnFirstPage( )
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

void MainWnd::OnLastPage( )
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

void MainWnd::OnViewSource( )
{
	Page *p = m_doc->GetPage( m_doc->CurrentPage() ) ;
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

void MainWnd::OnAppendPage( )
{
	m_doc->AppendPage( ) ;
	GoToPage( m_doc->PageCount() - 1 ) ;
}

void MainWnd::ConnectToolbarSignals( )
{
	// pointer tool
	connect(
		m_action_pointer_tool, 
		SIGNAL( activated( ) ), 
		m_view, 
		SLOT( OnSelectPointerTool( ) ) );

	// text tool
	connect(
		m_action_text_tool, 
		SIGNAL( activated( ) ), 
		m_view, 
		SLOT( OnSelectTextTool( ) ) );

	// zoom tool
	connect(
		m_action_zoom_tool, 
		SIGNAL( activated( ) ), 
		m_view, 
		SLOT( OnSelectZoomTool( ) ) );
}

void MainWnd::OnFileChanged( )
{
	setWindowModified(false);
	QString window_name = tr("Untitled");
	QString current_file = m_doc->GetCurrentFile();

	if ( !current_file.isEmpty() )
		window_name = QFileInfo(current_file).fileName();

	setWindowTitle( tr("%1[*] - %2")
						.arg(window_name)
						.arg(tr("SumPDF")) );
}

} // end of namespace
