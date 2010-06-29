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

// libpdfdoc headers
#include <libpdfdoc.hh>
#include <font/Font.hh>
#include <graphics/Color.hh>
#include <graphics/TextLine.hh>
#include <graphics/TextState.hh>
#include <util/Debug.hh>
#include <util/Matrix.hh>

// Qt headers
#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QPen>

// boost headers
#include <boost/format.hpp>

namespace pdf {

/**	constructor
*/
TextLineObject::TextLineObject( const TextLine *blk, QGraphicsItem *parent )
	: QAbstractGraphicsShapeItem( parent )
//	, m_line( blk->Clone() )
	, m_format( blk->Format() )
{
	blk->VisitChars( this ) ;
	m_bound = childrenBoundingRect( ) ;
	
	setTransform( ToQtMatrix( blk->Transform() ) ) ;
}

TextLineObject::TextLineObject(
	const GraphicsState&	format,
	const Matrix&			transform,
	const QString&			text,
	QGraphicsItem 			*parent )
	: QAbstractGraphicsShapeItem( parent )
	, m_format( format )
{
	if ( !text.isEmpty() )
	{
		std::auto_ptr<TextLine> line( CreateTextLine( format, transform,
			ToWStr(text) ) ) ;

		line->VisitChars( this ) ;
	}
	
	m_bound = childrenBoundingRect( ) ;
	
	setTransform( ToQtMatrix( transform ) ) ;
}

TextLineObject::~TextLineObject( )
{
}

void TextLineObject::AddString( const QString& str )
{
}

void TextLineObject::OnChar(
	wchar_t 			ch,
	double				offset,
	const Glyph			*glyph,
	const TextState&	state ) 
{
	GlyphGraphicsItem *item = new GlyphGraphicsItem( glyph, ch, this ) ;

	// colors
	QtGraphicsState gs( m_format ) ;
	item->setBrush( gs.Brush() ) ;
	
	// set glyph offset
	item->setPos( offset, 0 ) ;
	
	// scale font by their font size
	item->scale( state.ScaleFactor(), state.ScaleFactor() ) ;
}

GraphicsState TextLineObject::Format( ) const
{
	return m_format ;
}

void TextLineObject::AddChar( double offset, wchar_t ch )
{
	PDF_ASSERT( m_format.FontFace() != 0 ) ;
	
	const Font	*font	= m_format.FontFace() ;
	const Glyph	*glyph	= (font != 0) ? font->GetGlyph( ch ) : 0 ;
	
	if ( glyph != 0 )
		OnChar( ch, offset, glyph, Format().Text() ) ;

	m_bound = QRect( ) ;
}

std::auto_ptr<TextLine> TextLineObject::GetLine( ) const
{
	PDF_ASSERT( m_format.FontFace() != 0 ) ;
	
	std::auto_ptr<TextLine> line( CreateTextLine(
		m_format,
		Matrix::Translation( x(), y() ) * FromQtMatrix(transform()) ) ) ;
	
	foreach ( const QGraphicsItem *item, childItems() )
	{
		const GlyphGraphicsItem *glyph =
			dynamic_cast<const GlyphGraphicsItem*>( item ) ;
		
		line->AddChar( glyph->pos().x(), glyph->Char().unicode() ) ;
	}
	
	return line ;
}

// virtual functions for QGraphicsItem
QRectF TextLineObject::boundingRect( ) const
{
	if ( m_bound.isNull() )
		m_bound = childrenBoundingRect( ) ;

	return m_bound ;
}

void TextLineObject::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget 						*widget )
{
}

} // end of namespace
