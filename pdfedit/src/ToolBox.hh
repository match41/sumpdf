/***************************************************************************
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
	\file	ToolBox.hh
	\brief	definition the ToolBox class
	\date	March 25, 2010
*/

#ifndef __PDF_ToolBox_HH_EADER_INCLUDED__
#define __PDF_ToolBox_HH_EADER_INCLUDED__

#include <QToolBar>
#include <QPushButton>
#include <QMainWindow>
#include <QFontComboBox>
#include <QComboBox>
#include <QWidget>
#include <QDialog>

class QPushButton;
class QMainWindow;
class QFontComboBox;
class QComboBox;
class QWidget;
class QDialog;

namespace pdf {

class ToolBox 
	: public QDialog
{
	Q_OBJECT

public:
	explicit ToolBox( QToolBar *parent =0 , QWidget *parentWnd = 0) ;
	~ToolBox( ) ;

public:
	QPushButton		*m_insert_text;
	QPushButton		*m_delete_item;

// signals:
public slots:
	void OnInsertBtnUp( );
	void OnClicked( );
	void OnDelete( );

signals:
	void clicked();
	void deleteitem( );

private:
	void CreateTextInsertToolbar( QToolBar *parent , QWidget *parentWnd );

private:
		QPointF		m_pos;			// mouse position
};

} // end of namespace

#endif // ToolBox_HH_
