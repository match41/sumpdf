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
#include <QMenu>
#include <QPainter>
#include <QColorDialog>
#include <QTextEdit>
#include <QTextBlock>
#include <QString>
#include <QDebug>
#include <QTextLayout>

#include <QPaintEngine>
#include <QPaintDevice>
#include <QTextItem>

#include <limits>

namespace pdf {

InsertTextDlg::InsertTextDlg( QWidget *parent )
	: QDialog( parent ),
	m_colorbox( new ColorComboBox() )
{
	setupUi( this ) ;
	// set up color combo box
	m_colorbox->Initialize( m_colorbox );
	gridLayout->addWidget( dynamic_cast<QToolButton*>( m_colorbox ), 0, 7 );

	// rename OK button to insert
	m_btn_insert->button(QDialogButtonBox::Ok)->setText("Insert");

	// set up the font size combo box
	for (int i = 8; i < 30; i = i + 2)
		m_fontsize->addItem( QString().setNum(i) );
	QIntValidator *validator = new QIntValidator( 2, 64, this );
	m_fontsize->setValidator( validator );

	// text bold
	connect( m_bold,		SIGNAL( clicked() ), this, SLOT( OnFontChanged() ) );	
	// text italic
	connect( m_italic,		SIGNAL( clicked() ), this, SLOT( OnFontChanged() ) );
	// text color changed
	connect( 
		m_colorbox,	
		SIGNAL( clicked( QColor ) ), 
		this, 
		SLOT( OnFontChanged( QColor ) ) );

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
}

QTextDocument* InsertTextDlg::GetText( )
{
	return m_text->document() ;
}

void InsertTextDlg::OnFontChanged( )
{
	QFont font = m_font->currentFont();
    font.setPointSizeF( m_fontsize->currentText().toDouble() );
    font.setWeight(m_bold->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(m_italic->isChecked());
	m_text->setCurrentFont ( font );
	m_text->setFocus();
}

void InsertTextDlg::OnFontChanged( QColor color )
{
    m_text->setTextColor( color );
	m_text->setFocus();
}

} // end of namespace
