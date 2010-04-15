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

/**	\file	GraphicsObject.cc
	\brief	implementation of the GraphicsObject class
	\date	Apr 8, 2010
	\author	Nestal Wan
*/

#include "GraphicsObject.hh"

#include "Util.hh"

#include <font/Font.hh>
#include <graphics/GraphicsState.hh>
#include <graphics/TextState.hh>
#include <graphics/Color.hh>
#include <util/Debug.hh>

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QTransform>

#include <boost/format.hpp>

namespace pdf {

/**	constructor
	
*/
GraphicsObject::GraphicsObject( QGraphicsItem *parent  )
	: QAbstractGraphicsShapeItem( parent )
{
	// setup flags
	setFlags( ItemIsSelectable | ItemIsMovable

		// ItemSendsGeometryChanges needs Qt 4.6 or better
#if QT_VERSION >= 0x040600
		| ItemSendsGeometryChanges
#endif
	) ;

}

int GraphicsObject::rowCount( const QModelIndex& parent ) const
{
	return 6 ;
}

int GraphicsObject::columnCount( const QModelIndex& parent ) const
{
	return 2 ;
}

QVariant GraphicsObject::data( const QModelIndex& index, int role ) const
{
	if ( role == Qt::DisplayRole )
	{
		if ( index.column() == 0 )
		{
			switch ( index.row() )
			{
			case 0: return tr( "Transform" ) ;
			case 1: return tr( "Position" ) ;
			case 2: return tr( "Font" ) ;
			case 3: return tr( "Size" ) ;
			case 4: return tr( "Fill Colour" ) ;
			case 5: return tr( "Stroke Colour" ) ;
			}
		}
		else
		{
			GraphicsState gs = Format() ;
			using boost::format ;
		
			Font *font = gs.GetFont() ;
		
			QTransform t = transform() ;
			switch ( index.row() )
			{
			// matrix
			case 0: return QString( "%1 %2 %3 %4 %5 %6" )
				% t.m11() % t.m12() % t.m21() % t.m22() % t.m31() % t.m32() ;

			// position
			case 1: return QString( "%1, %2" ) % pos().x() % pos().y() ;
			
			// font name
			case 2: return (font != 0 ? font->BaseName().c_str() : "") ;
			
			// font size
			case 3: return gs.GetTextState().FontSize( ) ;
			
			case 4: return (format("%1%") % gs.NonStrokeColour()).str().c_str();
			
			case 5: return (format("%1%") % gs.StrokeColour()).str().c_str();
			}
		}
	}
	return QVariant( ) ;
}

QVariant GraphicsObject::headerData(
	int				sect,
	Qt::Orientation	ori,
	int role ) const
{
	if ( ori == Qt::Horizontal && role == Qt::DisplayRole )
	{
		switch ( sect )
		{
		case 0: return tr( "Property" ) ;
		case 1: return tr( "Value" ) ;
		}
	}
	else if ( ori == Qt::Vertical )
	{
	}
	return QVariant( ) ;
}

QVariant GraphicsObject::itemChange(
	GraphicsItemChange	change,
	const QVariant& 	value )
{
	if ( change == ItemPositionChange || change == ItemMatrixChange )
	{
		emit dataChanged( index( 0, 1 ), index( 1, 1 ) ) ;
	}
	return QAbstractGraphicsShapeItem::itemChange( change, value ) ;
}

void GraphicsObject::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget 						* )
{
	if ( option->state & QStyle::State_Selected )
		DrawSelectedBox( painter ) ;
}

void GraphicsObject::DrawSelectedBox( QPainter *p ) const
{
	PDF_ASSERT( p != 0 ) ;

	p->setBrush( Qt::NoBrush ) ;
	p->drawRect( boundingRect() ) ;
}

} // end of namespace
