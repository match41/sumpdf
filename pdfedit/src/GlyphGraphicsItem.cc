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

/**	\file	GlyphGraphicsItem.cc
	\brief	implementation of the GlyphGraphicsItem class
	\date	Jan 16, 2010
	\author	Nestal Wan
*/

#include "GlyphGraphicsItem.hh"

#include FT_IMAGE_H
#include FT_OUTLINE_H

#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QPen>

#include <cassert>

namespace pdf {

struct GlyphGraphicsItem::Render
{
	GlyphGraphicsItem				*pthis ;
	QPainterPath					*painter ;
	const QStyleOptionGraphicsItem	*option ;
	QWidget							*widget ;
} ;

/**	constructor
*/
GlyphGraphicsItem::GlyphGraphicsItem( FT_GlyphSlot glyph )
	: m_metrics( glyph->metrics )
{
	FT_Error error = FT_Get_Glyph( glyph, &m_glyph ) ;
	if ( error != 0 )
		throw -1 ;
}

QRectF GlyphGraphicsItem::boundingRect() const
{
	return QRectF(
		m_metrics.horiBearingX / 64.0,
		-m_metrics.horiBearingY / 64.0,
		m_metrics.width / 64.0,
		m_metrics.height / 64.0 ) ;
}

void GlyphGraphicsItem::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget							*widget )
{
	assert( m_glyph->format == FT_GLYPH_FORMAT_OUTLINE ) ;

	FT_Outline_Funcs f =
	{
		&GlyphGraphicsItem::MoveTo,
		&GlyphGraphicsItem::LineTo,
		&GlyphGraphicsItem::QuadTo,
		&GlyphGraphicsItem::CubicTo,
		0, 0
	} ;
	
	QPainterPath path ;
	Render r = { this, &path, option, widget } ;
	
	FT_OutlineGlyph og = reinterpret_cast<FT_OutlineGlyph>( m_glyph ) ; 
	FT_Outline_Decompose( &og->outline, &f, &r ) ;
	
	painter->setRenderHint( QPainter::Antialiasing ) ;
	painter->setBrush(QColor(0, 0, 0));
	painter->setPen( QPen( Qt::NoPen ) ) ;
	painter->drawPath( path ) ;
	
	// debug lines
	painter->setBrush( QBrush( Qt::NoBrush ) );
	painter->setPen( QPen( ) ) ;
	painter->drawRect( boundingRect() ) ;
//	painter->drawLine( 0, 0, m_metrics.width / 64.0, 0 ) ;
//	painter->drawLine( 0, 0, 0, -m_metrics.height / 64.0 ) ;
}

double GlyphGraphicsItem::Left( ) const
{
	FT_BitmapGlyph bmp_glyph = reinterpret_cast<FT_BitmapGlyph>( m_glyph ) ;
	return bmp_glyph->left ;
}

double GlyphGraphicsItem::Top( ) const
{
	FT_BitmapGlyph bmp_glyph = reinterpret_cast<FT_BitmapGlyph>( m_glyph ) ;
	return bmp_glyph->top ;
}

int GlyphGraphicsItem::MoveTo( const FT_Vector* to, void *user )
{
	Render *r = reinterpret_cast<Render*>( user ) ;
	return r->pthis->MoveTo( to, r->painter ) ;
}

int GlyphGraphicsItem::MoveTo( const FT_Vector* to, QPainterPath *p )
{
	p->moveTo( Transform( to ) ) ;
	return 0 ;
}

int GlyphGraphicsItem::LineTo( const FT_Vector* to, void *user )
{
	Render *r = reinterpret_cast<Render*>( user ) ;
	return r->pthis->LineTo( to, r->painter ) ;
}

int GlyphGraphicsItem::LineTo( const FT_Vector* to, QPainterPath *p )
{
	p->lineTo( Transform( to ) ) ;
	return 0 ;
}

int GlyphGraphicsItem::QuadTo(
	const FT_Vector	*control,
	const FT_Vector	*to,
	void 			*user )
{
	Render *r = reinterpret_cast<Render*>( user ) ;
	return r->pthis->QuadTo( control, to, r->painter ) ;
}

int GlyphGraphicsItem::QuadTo(
	const FT_Vector	*control,
	const FT_Vector	*to,
	QPainterPath 	*p)
{
	p->quadTo(
		Transform( control ),
		Transform( to ) ) ;
	return 0 ;
}

int GlyphGraphicsItem::CubicTo(
	const FT_Vector	*control1,
	const FT_Vector	*control2,
	const FT_Vector	*to,
	void 			*user )
{
	Render *r = reinterpret_cast<Render*>( user ) ;
	return r->pthis->CubicTo( control1, control2, to, r->painter ) ;
}

int GlyphGraphicsItem::CubicTo(
	const FT_Vector	*control1,
	const FT_Vector	*control2,
	const FT_Vector	*to,
	QPainterPath	*p )
{
	p->cubicTo(
		Transform( control1 ),
		Transform( control2 ),
		Transform( to ) ) ;
	return 0 ;
}

QPointF GlyphGraphicsItem::Transform( const FT_Vector *p ) const
{
	return QPointF(
		(m_metrics.horiBearingX + p->x ) / 64.0,
		(-m_metrics.horiBearingY + m_metrics.height - p->y) / 64.0 ) ;
}

} // end of namespace
