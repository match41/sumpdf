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

/**
	\file	PropertiesDlg.cc
	\brief	definition the PropertiesDlg class
	\date	Dec 27, 2009
	\author	Nestal Wan
*/

#include "PropertiesDlg.hh"

#include <Doc.hh>
#include <DocInfo.hh>

#include <cassert>
#include <iostream>

namespace pdf {

PropertiesDlg::PropertiesDlg( Doc *doc, QWidget *parent )
	: QDialog( parent ),
	  m_doc( doc )
{
	assert( m_doc != 0 ) ;
	setupUi( this ) ;

	connect(
		m_button_box,
		SIGNAL(accepted()),
		this,
		SLOT(OnOK()) );

	DocInfo* info = m_doc->Info( ) ;
	assert( info != 0 ) ;

	m_producer->setText( info->Producer().c_str() ) ;
	m_creator->setText( info->Creator().c_str() ) ;
	m_author->setText( info->Author().c_str() ) ;
	m_subject->setText( info->Subject().c_str() ) ;
	m_keywords->setText( info->Keywords().c_str() ) ;
}

void PropertiesDlg::OnOK( )
{
	DocInfo* info = m_doc->Info( ) ;
	assert( info != 0 ) ;

	info->SetCreator( m_creator->text().toStdString() ) ;
	info->SetProducer( m_producer->text().toStdString() ) ;
	info->SetAuthor( m_author->text().toStdString() ) ;
	info->SetSubject( m_subject->text().toStdString() ) ;
	info->SetKeywords( m_keywords->text().toStdString() ) ;
}

} // end of namespace
