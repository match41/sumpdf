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
 \file	PropertiesDlg.hh
 \brief	definition the PropertiesDlg class
 \date	Dec 27, 2009
 \author	nestal
 */

#ifndef __PDF_PROPERTIESDLG_HH_EADER_INCLUDED__
#define __PDF_PROPERTIESDLG_HH_EADER_INCLUDED__

#include <QDialog>

// Qt designer generated files
#include "ui_Properties.h"

namespace pdf {

class Doc ;

class PropertiesDlg : public QDialog, private Ui::ProperitiesUI 
{
	Q_OBJECT
	
public:
	explicit PropertiesDlg( Doc *doc, QWidget *parent ) ;

public slots :
	void OnOK( ) ;

private :
	Doc *m_doc ;
} ;

} // end of namespace

#endif // PROPERTIESDLG_HH_
