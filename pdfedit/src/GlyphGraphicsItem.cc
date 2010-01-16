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

namespace pdf {

/**	constructor
*/
GlyphGraphicsItem::GlyphGraphicsItem( FT_GlyphSlot glyph, double size )
	: m_metrics( glyph->metrics ),
	  m_size( size )
{
	FT_Get_Glyph( glyph, &m_glyph ) ;
}

QRectF GlyphGraphicsItem::boundingRect() const
{

	return QRectF( m_metrics.horiBearingX, m_metrics.horiBearingY, m_metrics.width, m_metrics.height ) ;
}

void GlyphGraphicsItem::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget							*widget )
{
}

} // end of namespace
