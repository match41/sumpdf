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

/**	\file	TextLineObject.cc
	\brief	implementation of the GlyphGroup class
	\date	Jan 24, 2010
	\author	Nestal Wan
*/

#include "TextLineObject.hh"

#include "GlyphGraphicsItem.hh"
#include "QtGraphicsState.hh"
#include "Util.hh"

#include <graphics/Color.hh>
#include <graphics/TextLine.hh>
#include <graphics/TextState.hh>

#include <font/Font.hh>
#include <util/Debug.hh>
#include <util/Matrix.hh>

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QPen>

#include <boost/format.hpp>

namespace pdf {

/**	constructor
*/
TextLineObject::TextLineObject( const TextLine *blk, QGraphicsItem *parent )
	: QAbstractGraphicsShapeItem( parent )
	, m_line( blk->Clone() )
{
	m_line->VisitChars( this ) ;
	m_bound = childrenBoundingRect( ) ;
	
	setTransform( ToQtMatrix( m_line->Transform() ) ) ;
}

TextLineObject::TextLineObject(
	const GraphicsState&	format,
	const Matrix&			transform,
	const QString&			text,
	QGraphicsItem 			*parent )
	: QAbstractGraphicsShapeItem( parent )
	, m_line( CreateTextLine( format, transform, ToWStr(text) ) )
{
	m_line->VisitChars( this ) ;
	m_bound = childrenBoundingRect( ) ;
	
	setTransform( ToQtMatrix( m_line->Transform() ) ) ;
}

TextLineObject::~TextLineObject( )
{
}

void TextLineObject::OnChar(
	wchar_t 			ch,
	double				offset,
	const Glyph			*glyph,
	const TextState&	state ) 
{
	GlyphGraphicsItem *item = new GlyphGraphicsItem( glyph, this ) ;

	// colors
	QtGraphicsState gs( m_line->Format() ) ;
	item->setBrush( gs.Brush() ) ;
	
	// set glyph offset
	item->setPos( offset, 0 ) ;
	
	// scale font by their font size
	item->scale( state.ScaleFactor(), state.ScaleFactor() ) ;
}

GraphicsState TextLineObject::Format( ) const
{
	PDF_ASSERT( m_line.get() != 0 ) ;
	return m_line->Format() ;
}

// TODO: create text line by the children glyphs
std::auto_ptr<TextLine> TextLineObject::GetLine( ) const
{
	PDF_ASSERT( m_line.get() != 0 ) ;
	PDF_ASSERT( m_line->Format().FontFace() != 0 ) ;
	
	std::auto_ptr<TextLine> line( m_line->Clone() ) ;
	line->SetTransform( Matrix::Translation( x(), y() ) * m_line->Transform() );
	return line ;
}

// virtual functions for QGraphicsItem
QRectF TextLineObject::boundingRect( ) const
{
	return m_bound ;
}

void TextLineObject::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget 						*widget )
{
}

} // end of namespace
