/***************************************************************************
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
 ***************************************************************************/

/**
	\file	ColorComboBox.hh
	\brief	definition the ColorComboBox class
	\date	May 6, 2010
*/

#ifndef __PDF_COLORCOMBOBOX_HH_HEADER_INCLUDED__
#define __PDF_COLORCOMBOBOX_HH_HEADER_INCLUDED__

#include <QToolButton>

//class QToolButton;

namespace pdf {

class ColorComboBox :
	public QToolButton
{
	Q_OBJECT

public:
	explicit ColorComboBox( QWidget *parent = 0 ) ;

	void Initialize( QToolButton* box );

signals:
	void clicked( QColor );

private slots:
	void OnTextColorChanged( );
	void OnSetColor( );			// set color from QColorDialog

private:
	QAction	*m_text_action;

private:
	QIcon OnCreateColorButtonIcon( const QString& , QColor );
    QMenu *OnCreateColorMenu( const char*, QColor );	// text color selection
	QIcon OnCreateColorIcon( QColor );
};

} // end of namespace

#endif // COLORCOMBOBOX_HH
