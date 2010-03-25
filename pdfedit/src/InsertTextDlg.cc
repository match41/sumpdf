/***************************************************************************\
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
	\file	InsertTextDlg.cc
	\brief	definition the InsertTextDlg class
	\date	March 21, 2010
*/

#include "InsertTextDlg.hh"
#include <QPushButton>

namespace pdf {

InsertTextDlg::InsertTextDlg( QWidget *parent )
	: QDialog( parent )
{
	setupUi( this ) ;

	// set up the font size combo box
    for (int i = 8; i < 30; i = i + 2)
        m_fontsize->addItem(QString().setNum(i));
	QIntValidator *validator = new QIntValidator(2, 64, this);
    m_fontsize->setValidator(validator);

	connect(
		m_font,
		SIGNAL( currentFontChanged( QFont ) ),
		this ,
		SLOT( EmitFontChanged( QFont ) ) );
	connect(
		m_fontsize,
		SIGNAL( currentIndexChanged( int ) ),
		this ,
		SLOT( EmitFontChanged( int ) ) );
	connect(
		this,
		SIGNAL( ReceiveFontChanged( QFont ) ),
		m_font ,
		SLOT( setCurrentFont( QFont ) ) );
	connect(
		this,
		SIGNAL( ReceiveFontChanged( int ) ),
		m_fontsize ,
		SLOT( setCurrentIndex( int ) ) );

	connect(
		m_font,
		SIGNAL( currentFontChanged( QFont ) ),
		this ,
		SLOT( OnFontChanged( ) ) );
	connect(
		m_fontsize,
		SIGNAL( currentIndexChanged( int ) ),
		this,
		SLOT( OnFontChanged( ) ) );
	connect(	// Insert text into current scene
		m_btn_insert, 
		SIGNAL( clicked( ) ),	
		this, 
		SLOT( OnInsertTextNow( ) ) );
}

InsertTextDlg::~InsertTextDlg( )
{
}

void InsertTextDlg::OnMousePositionSet( QPointF new_pos )
{
	pos = new_pos;
}

QString	InsertTextDlg::GetFontSize( )
{
	return m_fontsize->currentText( );
}

QTextEdit* InsertTextDlg::GetText( )
{
	return m_text;
}

QPointF	InsertTextDlg::GetPosition( )
{
	return pos;
}

void InsertTextDlg::OnFontChanged( )
{
    QFont font = m_font->currentFont();
    font.setPointSize(m_fontsize->currentText().toInt());
//    font.setWeight(m_action_bold->isChecked() ? QFont::Bold : QFont::Normal);
//    font.setItalic(m_action_italic->isChecked());
//    font.setUnderline(m_action_underline->isChecked());
    m_text->setFont(font);

}

void InsertTextDlg::OnInsertTextNow( )
{
	emit OnInsertClicked( );
}

void InsertTextDlg::EmitFontChanged( QFont f )
{
	emit FontPropertiesChanged( f );
}

void InsertTextDlg::EmitFontChanged( int i )
{
	emit FontPropertiesChanged( i );
	focusNextChild();
}

void InsertTextDlg::SetFontChanged( QFont f)
{
	emit ReceiveFontChanged( f );
}

void InsertTextDlg::SetFontChanged( int i )
{
	emit ReceiveFontChanged( i );
}

void InsertTextDlg::closeEvent( QCloseEvent *e )
{
	QDialog::closeEvent( e );
	emit OnDlgClosed( );
}

} // end of namespace
