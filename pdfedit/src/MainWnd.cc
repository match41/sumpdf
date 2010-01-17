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
#include "GlyphGraphicsItem.hh"
#include "PropertiesDlg.hh"

// Qt headers
#include <QComboBox>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QFont>
#include <QList>
#include <QMessageBox>
#include <QPointF>
#include <QTransform>
#include <QToolBar>

#include <QDebug>

#include "TextEdit.hh"

// libpdfdoc headers
#include <libpdfdoc.hh>
#include <Doc.hh>
#include <page/Page.hh>
#include <font/Font.hh>
#include <util/Rect.hh>
#include <util/Matrix.hh>
#include <page/PageContent.hh>
#include <graphics/Text.hh>
#include <graphics/TextLine.hh>
#include <graphics/TextBlock.hh>

#include FT_ERRORS_H

#include <boost/bind.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>

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

	// initialize tool bar
	m_tool_bar->addAction( m_action_open ) ;
	
	m_zoom_box->addItem( "100%", 1.0 ) ;
	m_zoom_box->addItem( "200%", 2.0 ) ;
	connect(
		m_zoom_box,
		SIGNAL(currentIndexChanged(int)),
		this,
		SLOT(OnToolZoom(int)) ) ;
	
	m_tool_bar->addWidget( m_zoom_box ) ;
}

/**	destructor is for the auto_ptr	
*/
MainWnd::~MainWnd( )
{
}

void MainWnd::OnToolZoom( int choice )
{
	qDebug() << "zoom to " << m_zoom_box->itemData( choice ).toDouble() ;
	m_view->Zoom( m_zoom_box->itemData( choice ).toDouble() ) ;
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
		
		PageContent *c = p->GetContent( ) ;
		c->VisitGraphics( this ) ;
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
	const TextBlock& b = *line.begin() ;

	FT_Face face = b.Format().GetFont()->Face( ) ;
	
	Matrix tm = line.Transform() ;
	const std::wstring& text = b.Text() ;
	for ( std::size_t i = 0 ; i < text.size() ; i++ )
	{
		int glyph_index = FT_Get_Char_Index( face, text[i] ) ; 

		// we want to do the scaling in double instead of inside freetype
		// in small font we don't have hinting
		FT_Error error = FT_Load_Glyph( face, glyph_index, FT_LOAD_NO_SCALE ) ;
		if ( error == FT_Err_Ok )
		{
			GlyphGraphicsItem *item = new GlyphGraphicsItem( face->glyph ) ;

			// scale font by their font size
			double scalefactor = b.Format().FontSize() / face->units_per_EM ;
			item->setTransform( ToQtMatrix( tm ) ) ;
			item->scale( scalefactor, scalefactor ) ;
			tm.Dx( tm.Dx() + face->glyph->advance.x * scalefactor) ;

			m_scene->addItem( item ) ;
		}
	}
}

void MainWnd::VisitGraphics( Graphics *gfx )
{
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
		// we shouldn't do like this. we shouldn't create another doc.
		// we should re-use m_doc.
		std::auto_ptr<Doc> doc( CreateDoc( ) ) ;
		Page *page = doc->AppendPage( ) ;
		StorePage( m_scene, doc.get(), page ) ;
		
		doc->Write( fname.toStdString() ) ;
		
		// transfer to m_doc if it doesn't exists
		if ( m_doc.get() == 0 )
			m_doc = doc ;
	}
}

void MainWnd::StorePage( QGraphicsScene *scene, Doc *doc, Page *page )
{
	assert( scene != 0 ) ;
	assert( doc != 0 ) ;
	assert( page != 0 ) ;
	
//	Font *font = doc->CreateSimpleFont( "Arial" ) ;
	
	QList<QGraphicsItem *> items = scene->items() ;
	for ( QList<QGraphicsItem*>::iterator i  = items.begin() ;
	                                      i != items.end() ; ++i )
	{
/*		QGraphicsProxyWidget *text =
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
							edit->toPlainText().toUtf8().data() ) ;*/
	}
}

QTransform MainWnd::ToQtMatrix( const Matrix& m )
{
	// here we do a little transformation on the y coordinate.
	// the PDF origin is at the lower left corner and the 
	// y-coordinate increase upward. However, Qt's origin is at the upper
	// left corner and increase downward.
	return QTransform(
		m.M11(), m.M12(), m.M21(), m.M22(), m.Dx(),
		/*m_scene->height() - */m.Dy() ) ;
}

} // end of namespace
