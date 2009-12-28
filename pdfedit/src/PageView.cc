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
 \author	nestal
 */

#include "PageView.hh"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsTextItem>

namespace pdf {

PageView::PageView( QGraphicsScene *scene, QWidget *parent )
	: QGraphicsView( scene, parent )
{
}

void PageView::mousePressEvent( QMouseEvent *event )
{
	qDebug() << "mouse click" ;
	QPointF pos = mapToScene( event->pos() ) ;
	QGraphicsTextItem *item = scene()->addText( "haha" ) ;
	item->setPos( pos ) ;
}

} // end of namespace
