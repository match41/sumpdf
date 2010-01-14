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
	\file	MainWnd.hh
	\brief	definition the MainWnd class
	\date	Dec 27, 2009
	\author	Nestal Wan
*/

#ifndef __PDF_MAINWND_HH_EADER_INCLUDED__
#define __PDF_MAINWND_HH_EADER_INCLUDED__

#include <QMainWindow>
#include "ui_MainWnd.h"
#include <graphics/GraphicsVisitor.hh>

#include <QString>

#include <memory>

class QGraphicsScene ;
class QTransform ;

namespace pdf {

// widgets
class PageView ;

// pdfdoc classes
class Doc ;
class Page ;
class Matrix ;
class TextLine ;

class MainWnd :
	public QMainWindow,
	private Ui::MainWndUI,
	private GraphicsVisitor
{
	Q_OBJECT

public:
	explicit MainWnd( QWidget *parent = 0 ) ;
	~MainWnd( ) ;
	
	void OpenFile( const QString& file ) ;

public slots :
	void OnAbout( ) ;
	void OnOpen( ) ;
	void OnProperties( ) ;
	void OnSaveAs( ) ;

private :
	void StorePage( QGraphicsScene *scene, Doc *doc, Page *page ) ;
	void VisitText( Text *text ) ;
	void VisitGraphics( Graphics *gfx ) ;
	void LoadTextLine( const TextLine& line ) ;

	QTransform ToQtMatrix( const Matrix& m ) ;

private :
	std::auto_ptr<Doc>	m_doc ;
	
	QGraphicsScene	*m_scene ;
	PageView		*m_view ;
} ;

} // end of namespace

#endif // MAINWND_HH_
