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

/**
	\file	PageView.cc
	\brief	definition the PageView class
	\date	Dec 28, 2009
	\author	Nestal Wan
*/

#include "PageView.hh"

#include "DocModel.hh"
#include "InsertTextDlg.hh"
#include "Util.hh"

// libpdfdoc headers
#include <util/Debug.hh>

// Qt headers
#include <QGraphicsItem>
#include <QDebug>
#include <QMouseEvent>
#include <QStatusBar>
#include <QTransform>

namespace pdf {

PageView::PageView( QWidget	*parent, DocModel *doc )
	: QGraphicsView( parent )
	, m_tool( pointer )
	, m_status( 0 )
	, m_doc( doc )
{
	// default zoom is 100%
	Zoom( 1.0 ) ;
	setRenderHint( QPainter::Antialiasing ) ;
	
	setMouseTracking( true ) ;
}

void PageView::SetStatusBar( QStatusBar *status )
{
	m_status = status ;
}

void PageView::Zoom( double factor )
{
	double physical = physicalDpiX() / 72.0 ;

	QMatrix m ;
	m.scale( factor * physical, -factor * physical ) ;
	setMatrix( m ) ;
}

void PageView::mousePressEvent( QMouseEvent *event )
{
	QPointF pos = mapToScene( event->pos() ) ;
	
	if ( m_tool == pointer )
	{
		if ( event->button() == Qt::RightButton )
		{
			// TODO: create a right-click menu here
			// TODO: delete the item when the user selects delete from the menu
		}
	}
	else if ( m_tool == text )
	{
		// create a new caret. use auto_ptr to ensure it is deleted
		// at the end of this statement block.
		std::auto_ptr<QGraphicsItem> caret( InsertCaret( pos ) ) ;
		
		// insert text from modal InsertTextDlg
		InsertTextDlg dlg( this );
		if ( dlg.exec() == QDialog::Accepted )
		{
			// user press OK. insert text here
			QTextEdit *text = dlg.GetText( ) ;
			m_doc->AddText(
				text->currentFont(), 
				dlg.GetFontSize().toDouble(),
				pos, 
				text->toPlainText(), 
				text->textColor() ) ;

		}
		
		// auto_ptr will delete the caret here, even if exception is thrown.
	}
	else if ( m_tool == zoom )
	{
		// TODO: insert zoom action here
		if ( event->button() == Qt::LeftButton )
		{
			// TODO: zoom in
		}
		else if ( event->button() == Qt::RightButton )
		{
			// TODO: zoom out
		}
	}

	QGraphicsView::mousePressEvent( event ) ;
}

void PageView::mouseMoveEvent( QMouseEvent *event )
{
	QPointF pos = mapToScene( event->pos() ) ;
	
	if ( m_status != 0 )
		m_status->showMessage( QString("%1,%2").arg( pos.x() ).arg( pos.y() ) ) ;

	QGraphicsView::mouseMoveEvent( event ) ;
}

QGraphicsItem* PageView::InsertCaret( QPointF pos )
{
	// TODO: no need to create text edit here
	QTextEdit text;
	text.setFontPointSize(12);
	text.setText("I");
	
	// TODO: try to use text and QFont directly here
	QGraphicsItem *item = scene()->addText(
		text.toPlainText(),
		text.currentFont() ) ;
	
	// TODO: can we not hard code it? the offset is different in different
	// systems.
	item->setPos( pos - QPoint(10,13) );	// (10,13) = correction offset
	
	return item ;
}

void PageView::DeleteItem( )
{
	foreach (QGraphicsItem *item, scene()->selectedItems())
	{
		scene()->removeItem( item );
	}
}

void PageView::OnSelectPointerTool( )
{
	m_tool = pointer ;
	setCursor( Qt::ArrowCursor ) ;
}

void PageView::OnSelectTextTool( )
{
	m_tool = text ;
	setCursor( Qt::IBeamCursor ) ;
}

void PageView::OnSelectZoomTool( )
{
	m_tool = zoom ;
	
	// TODO: use ":/images/Magnifying_glass_icon.svg" but smaller
	setCursor( Qt::CrossCursor ) ;
}

} // end of namespace
