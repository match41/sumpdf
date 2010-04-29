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
	\file	ToolBox.cc
	\brief	definition the ToolBox class
	\date	March 25, 2010
*/

#include "ToolBox.hh"
#include "InsertTextDlg.hh"
#include "MainWnd.hh"

namespace pdf {

ToolBox::ToolBox( QToolBar *parent, QWidget *parentWnd )
	: QDialog( parentWnd )
	, m_insert_text( new QPushButton )
	, m_delete_item( new QPushButton )
{
	CreateTextInsertToolbar( parent, parentWnd );
}

ToolBox::~ToolBox( )
{
}

void ToolBox::OnInsertBtnUp( )
{
	m_insert_text->setChecked( false );
}

void ToolBox::CreateTextInsertToolbar( QToolBar *parent, QWidget *parentWnd )
{
	// insert text push btn
	parent->addWidget( m_insert_text );	
	m_insert_text->setCheckable( true );
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/inserttext.png"), QSize(), QIcon::Normal, QIcon::Off);
	m_insert_text->setIcon(icon);

	// insert delete graphics item button
	parent->addWidget( m_delete_item );	
    icon.addFile(QString::fromUtf8(":/images/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
	m_delete_item->setIcon(icon);

	connect( m_insert_text,		SIGNAL( clicked() ),	this, SLOT( OnClicked() ) );
	connect( m_delete_item,		SIGNAL( clicked() ),	this, SLOT( OnDelete() ) );
}

void ToolBox::OnClicked( )
{
	emit clicked();
}

void ToolBox::OnDelete( )
{
	emit deleteitem( );
}

} // end of namespace
