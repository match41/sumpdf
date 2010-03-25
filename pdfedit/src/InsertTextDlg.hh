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
	\file	InsertTextDlg.hh
	\brief	definition the InsertTextDlg class
	\date	March 21, 2010
*/

#ifndef __PDF_INSERTTEXTWND_HH_EADER_INCLUDED__
#define __PDF_INSERTTEXTWND_HH_EADER_INCLUDED__

#include <QDialog>
#include "ui_InsertTextDlg.h"

class QPushButton;

namespace pdf {

class MainWnd;

class InsertTextDlg :
	public QDialog,
	private Ui::InsertTextUI
{
	Q_OBJECT

public:
	explicit InsertTextDlg( QWidget *parent ) ;
	~InsertTextDlg( ) ;

	QTextEdit*	GetText( );
	QString		GetFontSize( );
	QPointF		GetPosition( );

private:
	void closeEvent(QCloseEvent *e);

signals:
	void OnDlgClosed( );
	void OnInsertClicked( );
	void FontPropertiesChanged( QFont );
	void FontPropertiesChanged( int );
	void ReceiveFontChanged( QFont );
	void ReceiveFontChanged( int );

private slots:
	void OnFontChanged( ) ;		// apply font selection to QTextEdit
	void OnInsertTextNow( );	// insert text into document page
	void OnMousePositionSet( QPointF );
	void EmitFontChanged( QFont );		//	emit if Font changed
	void EmitFontChanged( int );		// emit if font size changed
	void SetFontChanged( QFont );
	void SetFontChanged( int );

private:
	QPushButton	*m_btn;
	QPointF		pos;			// mouse position

};

} // end of namespace

#endif // INSERTTEXTWND_HH_
