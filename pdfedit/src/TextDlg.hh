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

/**	\file	TextDlg.hh
    \brief	definition the TextDlg class
    \date	Mar 16, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTDLG_HH_EADER_INCLUDED__
#define __PDF_TEXTDLG_HH_EADER_INCLUDED__

#include "ui_TextDlg.h"
#include <QDialog>

namespace pdf {

///	brief description
/**	\internal
	The TextDlg class represents
*/
class TextDlg :
	public QDialog,
	public Ui::TextDlgUI
{
public :
	Q_OBJECT

public :
	explicit TextDlg( const char *msg, QWidget *parent = 0 ) ;
} ;

} // end of namespace

#endif // TEXTDLG_HH_
