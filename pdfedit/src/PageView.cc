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
#include "InsertTextDlg.hh"

// Qt
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
	else 	// click at empty space
	{
		if (QApplication::overrideCursor()->shape()== Qt::IBeamCursor) 
		{
			// insert text from modal InsertTextDlg
			QApplication::setOverrideCursor( QCursor(Qt::ArrowCursor) );
			InsertCaret( pos );
			InsertTextDlg dlg(this, pos);
			if ( dlg.exec() == QDialog::Accepted )	// insert text here
			{
				m_txt=dlg.GetText();
				emit InsertText( pos, dlg.GetFontSize().toDouble() );
			}
			DeleteCaret( pos );
			emit InsertBtnUp();
		}
	}

	QGraphicsView::mousePressEvent( event ) ;
}

void PageView::mouseMoveEvent( QMouseEvent *event )
{
	QPointF pos = mapToScene( event->pos() ) ;
	m_parent->statusBar()->showMessage( QString("%1,%2").arg( pos.x() ).arg( pos.y() ) ) ;

	QGraphicsView::mouseMoveEvent( event ) ;
}

void PageView::InsertCaret( QPointF pos)
{
	QTextEdit text;
	text.setFontPointSize(12);
	text.setText("I");
	QGraphicsItem *item = scene()->addText(text.toPlainText(),text.currentFont());
	// QMatrix m;
	//	m.scale(1,-1);
	//	item->setMatrix( m );
	item->setPos( pos - QPoint(10,13) );	// (10,13) = correction offset
}

void PageView::DeleteCaret( QPointF pos )
{
	QGraphicsItem *item = scene()->itemAt( pos - QPoint(10,13) );
	item->~QGraphicsItem( );
}

void PageView::DeleteItem( )
{
	foreach (QGraphicsItem *item, scene()->selectedItems())
	{
		scene()->removeItem( item );
	}
}

QTextEdit* PageView::GetText( )
{
	return m_txt;
}

} // end of namespace
