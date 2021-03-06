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

/**	\file	TextDlg.cc
	\brief	implementation of the TextDlg class
	\date	Mar 16, 2010
	\author	Nestal Wan
*/

#include "TextDlg.hh"

namespace pdf {

/**	constructor
	
*/
TextDlg::TextDlg( const char *msg, QWidget *parent )
	: QDialog( parent )
{
	setupUi( this ) ;
	
	m_data->setPlainText( msg ) ;
}

} // end of namespace
