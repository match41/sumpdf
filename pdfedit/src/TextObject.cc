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

/**	\file	GlyphGroup.cc
	\brief	implementation of the GlyphGroup class
	\date	Jan 24, 2010
	\author	Nestal Wan
*/

#include "TextObject.hh"

#include "GlyphGraphicsItem.hh"
#include "Util.hh"

#include <graphics/Color.hh>
#include <graphics/TextLine.hh>
#include <graphics/TextState.hh>

#include "Util.hh"

#include <font/Font.hh>
#include <util/Debug.hh>
#include <util/Matrix.hh>

#include <QColor>
#include <QBrush>
#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <boost/format.hpp>

namespace pdf {

/**	constructor
*/
TextObject::TextObject( const TextLine& blk, QGraphicsItem *parent )
	: QAbstractGraphicsShapeItem( parent )
	, m_line( blk )
{
	m_line.VisitChars( this ) ;
	m_bound = childrenBoundingRect( ) ;
	
	// setup flags
	setFlags( ItemIsSelectable | ItemIsMovable

		// ItemSendsGeometryChanges needs Qt 4.6 or better
#if QT_VERSION >= 0x040600
		| ItemSendsGeometryChanges
#endif
	) ;

	QTransform t = ToQtMatrix( m_line.Transform() ) ;
	setTransform( t ) ;
}

void TextObject::OnChar(
	wchar_t 			ch,
	double				offset,
	const Glyph			*glyph,
	const TextState&	state ) 
{
	GlyphGraphicsItem *item = new GlyphGraphicsItem( glyph, this ) ;

	// fill colour
	QColor fill_color ;
	Color nstrk = m_line.Format().NonStrokeColour() ;
	item->setBrush( QBrush( ToQColor( m_line.Format().NonStrokeColour() ) ) ) ;
	
	// set glyph offset
	item->setPos( offset, 0 ) ;
	
	// scale font by their font size
	item->scale( state.ScaleFactor(), state.ScaleFactor() ) ;

//	addToGroup( item ) ;
}

int TextObject::type( ) const
{
	return Type ;
}

const GraphicsState& TextObject::Format( ) const
{
	return m_line.Format() ;
}

TextLine TextObject::GetLine( ) const
{
	TextLine line( m_line ) ;
	line.SetTransform( Matrix::Translation( x(), y() ) * m_line.Transform() ) ;
	return line ;
}

int TextObject::rowCount( const QModelIndex& parent ) const
{
	return 6 ;
}

int TextObject::columnCount( const QModelIndex& parent ) const
{
	return 2 ;
}

QVariant TextObject::data( const QModelIndex& index, int role ) const
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
			const GraphicsState& gs = m_line.Format() ;
			using boost::format ;
		
			QTransform t = transform() ;
			switch ( index.row() )
			{
			// matrix
			case 0: return QString( "%1 %2 %3 %4 %5 %6" )
				% t.m11() % t.m12() % t.m21() % t.m22() % t.m31() % t.m32() ;

			// position
			case 1: return QString( "%1, %2" ) % pos().x() % pos().y() ;
			
			// font name
			case 2: return gs.GetFont()->BaseName().c_str() ;
			
			// font size
			case 3: return gs.GetTextState().FontSize( ) ;
			
			case 4: return (format("%1%") % gs.NonStrokeColour()).str().c_str();
			
			case 5: return (format("%1%") % gs.StrokeColour()).str().c_str();
			}
		}
	}
	return QVariant( ) ;
}

QVariant TextObject::headerData( int sect, Qt::Orientation ori, int role ) const
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

QVariant TextObject::itemChange( GraphicsItemChange change, const QVariant& value )
{
	if ( change == ItemPositionChange || change == ItemMatrixChange )
	{
		emit dataChanged( index( 0, 1 ), index( 1, 1 ) ) ;
	}
	return QGraphicsItem::itemChange( change, value ) ;
}

// virtual functions for QGraphicsItem
QRectF TextObject::boundingRect( ) const
{
	return m_bound ;
}

void TextObject::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget 						* )
{
	if ( option->state & QStyle::State_Selected )
	{
		painter->setBrush( Qt::NoBrush ) ;
		painter->drawRect( m_bound ) ;
	}
}

} // end of namespace
