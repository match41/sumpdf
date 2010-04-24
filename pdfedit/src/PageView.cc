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

#include "TextEdit.hh"

#include "Util.hh"

#include <util/Debug.hh>

#include <QGraphicsItem>
#include <QDebug>
#include <QMainWindow>
#include <QMouseEvent>
#include <QStatusBar>
#include <QMessageBox>
#include <QTransform>
#include <QVariant>

namespace pdf {

PageView::PageView( QMainWindow	*parent )
	: QGraphicsView( parent )
	, m_parent( parent )
{
	PDF_ASSERT( parent != 0 ) ;

	// default zoom is 100%
	Zoom( 1.0 ) ;
	setRenderHint( QPainter::Antialiasing ) ;
	
	setMouseTracking( true ) ;
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
	
	QGraphicsItem *item = scene()->itemAt( pos ) ;
	
	// add text box if the user clicks empty space
	if ( event->button() == Qt::RightButton &&
	     item != 0  )
	{
		QTransform t = item->transform( ) ;		
		QMessageBox::information(
			this,
			"Item",
			QString( "%1 %2 %3 %4 %5 %6" )
			% t.m11() % t.m12() % t.m21() % t.m22() % t.m31() % t.m32() ) ;
	}
	
	// click at empty space
	else
	{
		emit mousePositionSet(pos);	// mouse position at empty space
	}
	

	QGraphicsView::mousePressEvent( event ) ;
}

void PageView::mouseMoveEvent( QMouseEvent *event )
{
	QPointF pos = mapToScene( event->pos() ) ;
	m_parent->statusBar()->showMessage( QString("%1,%2").arg( pos.x() ).arg( pos.y() ) ) ;

	QGraphicsView::mouseMoveEvent( event ) ;
}

void PageView::InsertI_beam( QPointF pos)
{
	QTextEdit text;
	text.setFontPointSize(16);
	text.setText("I");
	QGraphicsItem *item = scene()->addText(text.toPlainText(),text.currentFont());
	QMatrix m;
//	m.scale(1,-1);
//	item->setMatrix( m );
	QPoint pos_correction(10.0,13.0);
	item->setPos( pos - pos_correction);
	//	->itemAt( pos ) ;
}

void PageView::DeleteI_beam( QPointF pos )
{
	QPoint pos_correction(10.0,13.0);
	QGraphicsItem *item = scene()->itemAt( pos - pos_correction );
	item->hide();
}

} // end of namespace
