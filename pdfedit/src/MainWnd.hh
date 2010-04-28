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

#include <QString>

#include <memory>

class QGraphicsScene ;
class QTransform ;
class QToolBar ;
class QComboBox ;

class QPushButton;
class QLabel;
class QTextEdit;
class QFontComboBox;


namespace pdf {

// widgets
class PageView ;

// pdfdoc classes
class Doc ;
class Page ;
class Matrix ;
class TextLine ;
class InsertTextDlg;
class DocModel ;
class ToolBox;

class MainWnd :
	public QMainWindow,
	private Ui::MainWndUI
{
	Q_OBJECT

public:
	explicit MainWnd( QWidget *parent = 0 ) ;
	~MainWnd( ) ;

	void OpenFile( const QString& file ) ;

public slots :
	void OnAbout( ) ;
	void OnNew( ) ;
	void OnOpen( ) ;
	void OnProperties( ) ;
	void OnSaveAs( ) ;
	void OnToolZoom( int choice ) ;

	void OnNextPage( ) ;
	void OnPreviousPage( ) ;
	void OnLastPage( );
	void OnFirstPage( );

	void GoToPage( std::size_t index ) ;
	void OnSelectionChanged( ) ;
	void OnChanged( const QList<QRectF>& ) ;
	void OnViewSource( ) ;

	void OnInsertDlg( );
	void OnInsertTextNow( );

	void OnFileChanged( );

private :
	void LoadTextLine( const TextLine& line ) ;
	void TextInsertConnect( );

private :
	DocModel		*m_doc ;

	PageView		*m_view ;
	QToolBar		*m_tool_bar ;
	QComboBox 		*m_zoom_box ;

	QLabel			*m_label;
	QTextEdit		*m_text;

	// text editing
	QTextEdit		*m_text_edit;
	InsertTextDlg	*m_insert_dlg;

	ToolBox		*m_texttb_items;
} ;

} // end of namespace

#endif // MAINWND_HH_
