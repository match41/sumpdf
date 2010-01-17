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

#include <QGraphicsItem>
#include <QDebug>
#include <QMouseEvent>

namespace pdf {

PageView::PageView( QGraphicsScene *scene, QWidget *parent )
	: QGraphicsView( scene, parent )
{
//	scale( 1.0/64.0 * 4, -1.0/64.0 * 4 ) ;
	scale( 5, -5 ) ;
	setRenderHint( QPainter::Antialiasing ) ;
}

void PageView::mousePressEvent( QMouseEvent *event )
{
	QPointF pos = mapToScene( event->pos() ) ;
	
	// add text box if the user clicks empty space
	if ( QGraphicsItem *item = scene()->itemAt( pos ) )
	{
		qDebug() << item->mapFromParent( pos ) ;
	}

	QGraphicsView::mousePressEvent( event ) ;
}

void PageView::drawForeground( QPainter * painter, const QRectF & rect )
{
	QGraphicsView::drawForeground( painter, rect ) ;
}

} // end of namespace
