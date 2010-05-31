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

/**	\file	GraphicsDelegate.hh
    \brief	definition the GraphicsDelegate class
    \date	May 31, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GRAPHICSDELEGATE_HEADER_INCLUDED__
#define __PDF_GRAPHICSDELEGATE_HEADER_INCLUDED__

#include <QItemDelegate>

namespace pdf {

///	brief description
/**	\internal
	The GraphicsDelegate class represents
*/
class GraphicsDelegate : public QItemDelegate
{
	Q_OBJECT

public :
	explicit GraphicsDelegate( QObject *parent = 0 ) ;

	QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	
	void setEditorData( QWidget *editor, const QModelIndex& index ) const ;
	void setModelData( QWidget *editor, QAbstractItemModel *model,
		const QModelIndex& index ) const ;
	
	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex& index ) const ;
} ;

} // end of namespace

#endif // GRAPHICSDELEGATE_HH_
