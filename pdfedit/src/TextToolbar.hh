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
	\file	TextToolbar.hh
	\brief	definition the TextToolbar class
	\date	March 25, 2010
*/

#ifndef __PDF_TEXTTOOLBAR_HH_EADER_INCLUDED__
#define __PDF_TEXTTOOLBAR_HH_EADER_INCLUDED__

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

class TextToolbar 
	: public QDialog
{
	Q_OBJECT

public:
	explicit TextToolbar( QToolBar *parent =0 , QWidget *parentWnd = 0) ;
	~TextToolbar( ) ;

public:
	QPushButton		*m_insert_text;
	// QFontComboBox	*m_font;
	// QComboBox		*m_font_size;

// signals:

public slots:
	void OnInsertBtnUp( );

signals:
	void clicked( );

private:
	void CreateTextInsertToolbar( QToolBar *parent , QWidget *parentWnd );

};

} // end of namespace

#endif // TEXTTOOLBAR_HH_
