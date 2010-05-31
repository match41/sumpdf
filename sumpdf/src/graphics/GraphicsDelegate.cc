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

/**	\file	GraphicsDelegate.cc
	\brief	implementation of the GraphicsDelegate class
	\date	May 31, 2010
	\author	Nestal Wan
*/

#include "GraphicsDelegate.hh"

#include "ColorButton.hh"

namespace pdf {

/**	constructor
	
*/
GraphicsDelegate::GraphicsDelegate( QObject *parent )
	: QItemDelegate( parent )
{
}

QWidget* GraphicsDelegate::createEditor(
	QWidget						*parent,
	const QStyleOptionViewItem&	option,
	const QModelIndex&			index ) const
{
	switch ( index.row() )
	{
	case 4:
	case 5:		return new ColorButton( parent ) ;
	default :	return QItemDelegate::createEditor( parent, option, index ) ;
	}
}

void GraphicsDelegate::setEditorData(
	QWidget				*editor,
	const QModelIndex&	index ) const
{
}

void GraphicsDelegate::setModelData(
	QWidget 			*editor,
	QAbstractItemModel	*model,
	const QModelIndex&	index ) const
{
}

void GraphicsDelegate::updateEditorGeometry(
	QWidget 					*editor,
	const QStyleOptionViewItem&	option,
	const QModelIndex&			index ) const
{
	editor->setGeometry( option.rect ) ;

}

} // end of namespace
