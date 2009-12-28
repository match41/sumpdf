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

// Qt headers
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QList>
#include <QMessageBox>
#include <QPointF>
#include "TextEdit.hh"

// libpdfdoc headers
#include <libpdfdoc.hh>
#include <Doc.hh>
#include <Page.hh>
#include <Rect.hh>

#include <cassert>

namespace pdf {

MainWnd::MainWnd( QWidget *parent )
	: QMainWindow( parent ),
	  m_scene( new QGraphicsScene( QRectF( 0, 0, 595, 842 ), this ) ),
	  m_view( new PageView( m_scene, this ) )
{
	setupUi( this ) ;
	setCentralWidget( m_view ) ;
	
	connect( m_action_about, SIGNAL(triggered()), this, SLOT(OnAbout()));
	connect(
		m_action_properties,
		SIGNAL(triggered()),
		this,
		SLOT(OnProperties()) );
	connect(
		m_action_open,
		SIGNAL(triggered()),
		this,
		SLOT(OnOpen()) );
	connect(
		m_action_save_as,
		SIGNAL(triggered()),
		this,
		SLOT(OnSaveAs()) );
}
/**	destructor is for the auto_ptr	
*/
MainWnd::~MainWnd( )
{
}

void MainWnd::OpenFile( const QString& file )
{
	m_doc.reset( CreateDoc( ) ) ;
	m_doc->Read( file.toStdString() ) ;
	
	if ( m_doc->PageCount() > 0 )
	{
		Page *p = m_doc->GetPage( 0 ) ;
		Rect r = p->MediaBox( ) ;
		m_scene->setSceneRect( 0, 0, r.Width(), r.Height() ) ;
	}
}

void MainWnd::OnAbout( )
{
	QMessageBox::information( this, "About PDF Editor",
		"PDF Editor version 0.0.1" ) ;
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
		std::auto_ptr<Doc> doc( CreateDoc( ) ) ;
		Page *page = doc->AppendPage( ) ;
		StorePage( m_scene, doc.get(), page ) ;
		
		doc->Write( fname.toStdString() ) ;
	}
}

void MainWnd::StorePage( QGraphicsScene *scene, Doc *doc, Page *page )
{
	assert( scene != 0 ) ;
	assert( doc != 0 ) ;
	assert( page != 0 ) ;
	
	Font *font = doc->CreateSimpleFont( "Arial" ) ;
	
	QList<QGraphicsItem *> items = scene->items() ;
	for ( QList<QGraphicsItem*>::iterator i  = items.begin() ;
	                                      i != items.end() ; ++i )
	{
		QGraphicsProxyWidget *text =
			qgraphicsitem_cast<QGraphicsProxyWidget*>( *i ) ;
		
		QPointF pos = text->scenePos( ) ;
		TextEdit *edit = qobject_cast<TextEdit*>( text->widget() ) ;
		assert( edit != 0 ) ;

		// here we do a little transformation on the y coordinate.
		// the PDF origin is at the lower left corner and the 
		// y-coordinate increase upward. However, Qt's origin is at the upper
		// left corner and increase downward.
		if ( edit != 0 )
			page->DrawText( pos.x(), scene->height() - pos.y(), font,
							edit->toPlainText().toUtf8().data() ) ;
	}
}

} // end of namespace
