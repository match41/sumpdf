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

#include <font/Glyph.hh>
#include <font/Outline.hh>

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
		p->quadTo( cx, cy, tx, ty ) ;
	}
	
	void CubicTo( int c1x, int c1y, int c2x, int c2y, int tx,  int ty )
	{
		p->cubicTo( c1x, c1y, c2x, c2y, tx, ty ) ;
	}
} ;

/**	constructor
*/
GlyphGraphicsItem::GlyphGraphicsItem( const Glyph *glyph )
	: m_glyph( glyph )
{
	QPainterPath path( QPointF ( 0.0, 0.0 ) ) ;
	Render r ;
	r.p = &path ;
	m_glyph->Decompose( &r ) ;
	
	setBrush( QColor(0, 0, 0) ) ;
	setPen( QPen( Qt::NoPen ) ) ;
	setPath( path ) ;
}

int GlyphGraphicsItem::type( ) const
{
	return Type ;
}

} // end of namespace
