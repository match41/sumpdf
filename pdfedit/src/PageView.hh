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
	\file	PageView.hh
	\brief	definition the PageView class
	\date	Dec 28, 2009
	\author	Nestal Wan
*/

#ifndef __PDF_PAGEVIEW_HEADER_INCLUDED__
#define __PDF_PAGEVIEW_HEADER_INCLUDED__

#include <QGraphicsView>

class QPainter ;

namespace pdf {

class Page ;

class PageView : public QGraphicsView
{
	Q_OBJECT

public:
	explicit PageView( QGraphicsScene *scene, QWidget *parent ) ;

	void Zoom( double factor ) ;

protected :
	void mousePressEvent( QMouseEvent *event ) ;

private :
	class LineEdit ;
} ;

} // end of namespace

#endif // PAGEVIEW_HH_
