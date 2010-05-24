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
#include <QPointF>

class QPointF ;
class QWidget ;
class QStatusBar ;

namespace pdf {

class DocModel ;

class PageView : public QGraphicsView
{
	Q_OBJECT

public:
	PageView( QWidget *parent, DocModel *doc ) ;

	void Zoom( double factor ) ;
	void SetStatusBar( QStatusBar *status ) ;

public slots:
	void OnSelectPointerTool( ) ;
	void OnSelectTextTool( ) ;
	void OnSelectZoomTool( ) ;
	void DeleteSelection( ) ;

protected :
	void mousePressEvent( QMouseEvent *event ) ;
	void mouseMoveEvent( QMouseEvent *event ) ;

private :
	QGraphicsItem* InsertCaret( QPointF pos ) ;
	
	// event handlers for tools
	void OnPointerLeftClick( QMouseEvent *event ) ;
	void OnPointerRightClick( QMouseEvent *event ) ;
	void OnTextLeftClick( QMouseEvent *event ) ;

private :
	enum Tool { pointer, text, zoom } m_tool ;

	QStatusBar	*m_status ;
	DocModel	*m_doc ;
	
	// dragging
	QPointF		m_start_drag ;
	double			m_page_size ;
} ;

} // end of namespace

#endif // PAGEVIEW_HH_
