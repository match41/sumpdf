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

/**	\file	ExceptionDlg.cc
	\brief	implementation of the ExceptionDlg class
	\date	Feb 2, 2010
	\author	Nestal Wan
*/

#include "ExceptionDlg.hh"

#include <util/Exception.hh>

#include <QApplication>
#include <QClipboard>

namespace pdf {

/**	constructor
	
*/
ExceptionDlg::ExceptionDlg( const Exception& e, QWidget *parent )
	: QDialog( parent )
	, m_what( e.what() )
{
	setupUi( this ) ;
	
	m_msg_edit->setPlainText( e.ErrorMessage().c_str() ) ;
	m_bt_edit->setPlainText( e.GetBacktrace().c_str() ) ;
	
	Init( ) ;
}

ExceptionDlg::ExceptionDlg( const char *what, QWidget *parent ) :
	QDialog( parent ),
	m_what( what )
{
	setupUi( this ) ;
	
	m_msg_edit->setPlainText( m_what ) ;
	Init( ) ;
}

void ExceptionDlg::Init( )
{	
	connect( m_ok_button,	SIGNAL(clicked()), this, SLOT(accept() ) ) ;
	connect( m_copy_button,	SIGNAL(clicked()), this, SLOT(OnCopy() ) ) ;
}

void ExceptionDlg::OnCopy( )
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText( m_what ) ;
}

} // end of namespace
