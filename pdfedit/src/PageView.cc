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
#include <QMenu>

#include <limits>

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

void PageView::OnPointerRightClick( QMouseEvent *event )
{
	// do the selection as well
	OnPointerLeftClick( event ) ;

	std::auto_ptr<QMenu> menu( new QMenu( this ) ) ;
	menu->addAction( "Delete", this, SLOT(DeleteSelection()) ) ;
	menu->popup( event->globalPos() ) ;
	menu->exec( ) ;
}

void PageView::OnPointerLeftClick( QMouseEvent *event )
{
	QPointF pos = mapToScene( event->pos() ) ;
	
	// first clear the selection
	scene()->clearSelection() ;
	
	QGraphicsItem *smallest = 0 ;
	double min_area = std::numeric_limits<double>::max() ;
	
	QList<QGraphicsItem*> selected = items( event->pos() ) ;
	for ( QList<QGraphicsItem*>::iterator i = selected.begin() ;
		i != selected.end() ; ++i )
	{
		QRectF bbox = (*i)->boundingRect() ;
		double area = bbox.width() * bbox.height() ;
		if ( area < min_area )
		{
			min_area = area ;
			smallest = *i ;
		}
	}
	
	if ( smallest != 0 )
	{
		smallest->setSelected(true) ;
		m_start_drag = pos ;
	}
}

void PageView::OnTextLeftClick( QMouseEvent *event )
{
	QPointF pos = mapToScene( event->pos() ) ;
	
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

void PageView::mousePressEvent( QMouseEvent *event )
{
	if ( m_tool == pointer )
	{
		if ( event->button() == Qt::RightButton )
			OnPointerRightClick( event ) ;
		
		else if ( event->button() == Qt::LeftButton )
			OnPointerLeftClick( event ) ;
	}
	else if ( m_tool == text )
	{
		if ( event->button() == Qt::LeftButton )
			OnTextLeftClick( event ) ;
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
}

void PageView::mouseMoveEvent( QMouseEvent *event )
{
	QPointF pos = mapToScene( event->pos() ) ;
	
	if ( m_status != 0 )
		m_status->showMessage( QString("%1,%2").arg( pos.x() ).arg( pos.y() ) ) ;

	// do draggin with pointer tool
	if ( m_tool == pointer && event->buttons() & Qt::LeftButton )
	{
		QPointF offset = pos - m_start_drag ;
		
		QList<QGraphicsItem*> sel = scene()->selectedItems( ) ;
		for ( QList<QGraphicsItem*>::iterator i = sel.begin() ;
			i != sel.end() ; ++i )
		{
			(*i)->moveBy( offset.x(), offset.y() ) ;
		}
		
		m_start_drag = pos ;
	}
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

void PageView::DeleteSelection( )
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
