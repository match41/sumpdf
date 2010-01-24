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

#include <font/Outline.hh>

//#include FT_IMAGE_H
//#include FT_OUTLINE_H

#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QPen>
#include <QApplication>

#include <util/Debug.hh>

namespace pdf {

struct GlyphGraphicsItem::Render : public Outline
{
	QPainterPath	*p ;

	void MoveTo( int x, int y )
	{
		p->moveTo( x, y ) ;
	}
	
	void LineTo( int x, int y )
	{
		p->lineTo( x, y ) ;
	}
	
	void QuadTo( int cx, int cy, int tx, int ty )
	{
//qDebug() << "cx = " << cx << " cy = " << cy << " tx = " << tx << " ty = " << ty ;
		p->quadTo( cx, cy, tx, ty ) ;
	}
	
	void CubicTo( int c1x, int c1y, int c2x, int c2y, int tx,  int ty )
	{
		p->cubicTo( c1x, c1y, c2x, c2y, tx, ty ) ;
	}
} ;

/**	constructor
*/
GlyphGraphicsItem::GlyphGraphicsItem( const Glyph& glyph )
	: m_glyph( glyph )
{
/*
	PDF_ASSERT( glyph != 0 ) ;
	PDF_ASSERT( glyph->format == FT_GLYPH_FORMAT_OUTLINE ) ;

	FT_Outline_Funcs f =
	{
		&GlyphGraphicsItem::MoveTo,
		&GlyphGraphicsItem::LineTo,
		&GlyphGraphicsItem::QuadTo,
		&GlyphGraphicsItem::CubicTo,
		0, 0
	} ;
*/
	
	QPainterPath path( QPointF ( 0.0, 0.0 ) ) ;
	Render r ;
	r.p = &path ;
	m_glyph.Decompose( &r ) ;
//	FT_OutlineGlyph og = reinterpret_cast<FT_OutlineGlyph>( glyph ) ; 
//	FT_Outline_Decompose( &og->outline, &f, &r ) ;
	
	setBrush( QColor(0, 0, 0) ) ;
	setPen( QPen( Qt::NoPen ) ) ;
	setPath( path ) ;
}

/*
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
	return QPointF( p->x, p->y ) ;
}
*/

} // end of namespace
