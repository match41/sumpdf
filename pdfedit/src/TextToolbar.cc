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
	\file	TextToolbar.cc
	\brief	definition the TextToolbar class
	\date	March 25, 2010
*/

#include "TextToolbar.hh"
#include "InsertTextDlg.hh"
#include "MainWnd.hh"

namespace pdf {

TextToolbar::TextToolbar( QToolBar *parent, QWidget *parentWnd )
	: QDialog( parentWnd )
	, m_insert_text(new QPushButton( "&Insert Text" ) )
{
	CreateTextInsertToolbar( parent, parentWnd );
}

TextToolbar::~TextToolbar( )
{
}

void TextToolbar::OnInsertBtnUp( )
{
	m_insert_text->setChecked( false );
}

void TextToolbar::CreateTextInsertToolbar( QToolBar *parent, QWidget *parentWnd )
{
	parent->addWidget( m_insert_text );	// insert text push btn
	m_insert_text->setCheckable( true );

	m_font = new QFontComboBox();	// font combo box
    m_font_size = new QComboBox();	// font size combo box

    m_font_size->setEditable(true);
    for (int i = 8; i < 30; i += 2)
        m_font_size->addItem( QString().setNum( i ) );
    QIntValidator *validator = new QIntValidator( 2, 64, parent );
    m_font_size->setValidator( validator );

	parent->addWidget( m_font );
	parent->addWidget( m_font_size );

	connect( m_insert_text,		SIGNAL( clicked() ),	parentWnd, SLOT(OnInsertDlg() ) );

}
} // end of namespace
