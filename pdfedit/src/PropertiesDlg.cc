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
	
	DocInfo* info = m_doc->Info( ) ;
	assert( info != 0 ) ;

std::cout << info->Producer() << std::endl ;

	m_producer->setText( info->Producer().c_str() ) ;
	m_creator->setText( info->Creator().c_str() ) ;
}

} // end of namespace
