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

/**	\file	GraphicsObject.hh
    \brief	definition the GraphicsObject class
    \date	Apr 8, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GRAPHICSOBJECT_HH_EADER_INCLUDED__
#define __PDF_GRAPHICSOBJECT_HH_EADER_INCLUDED__

#include <QAbstractGraphicsShapeItem>
#include <QAbstractTableModel>

namespace pdf {

class GraphicsState ;

///	brief description
/**	\internal
	The GraphicsObject class represents
*/
class GraphicsObject :
	public QAbstractGraphicsShapeItem,
	public QAbstractTableModel
{
public :
	explicit GraphicsObject( QGraphicsItem *parent = 0 ) ;

	//@{
	/// abstract graphics object pure members
	virtual GraphicsState Format() const = 0 ;
	//@}
	
	//@{
	/// abstract table model members
	int rowCount( const QModelIndex& parent ) const ;
	int columnCount( const QModelIndex& parent ) const ;
	
	QVariant data( const QModelIndex& index, int role ) const ;
	QVariant headerData( int sect, Qt::Orientation ori, int role ) const ;
	//@}

	QVariant itemChange( GraphicsItemChange change, const QVariant& value ) ;
	int type( ) const ;
	static const int Type = UserType + 1 ;

	virtual void paint(
		QPainter 						*painter,
		const QStyleOptionGraphicsItem	*option,
		QWidget 						*widget ) = 0 ; 
} ;

} // end of namespace

#endif // GRAPHICSOBJECT_HH_
