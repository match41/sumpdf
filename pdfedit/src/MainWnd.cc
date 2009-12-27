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

#include "PropertiesDlg.hh"

// Qt headers
#include <QFileDialog>
#include <QGraphicsView>
#include <QMessageBox>

// libpdfdoc headers
#include <libpdfdoc.hh>
#include <Doc.hh>
#include <Page.hh>
#include <Rect.hh>

#include <iostream>

namespace pdf {

MainWnd::MainWnd( QWidget *parent )
	: QMainWindow( parent ),
	  m_view( new QGraphicsView( &m_scene, this ) )
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
}

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
		m_scene.setSceneRect( 0, 0, r.Width(), r.Height() ) ;
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
	OpenFile( fname ) ;
}

} // end of namespace
