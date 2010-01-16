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

/**	\file	GlyphGraphicsItem.hh
    \brief	definition the GlyphGraphicsItem class
    \date	Jan 16, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GLYPHGRAPHICSITEM_HH_EADER_INCLUDED__
#define __PDF_GLYPHGRAPHICSITEM_HH_EADER_INCLUDED__

#include <QGraphicsItem>

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace pdf {

///	brief description
/**	The GlyphGraphicsItem class represents
*/
class GlyphGraphicsItem : public QGraphicsItem
{
public :
	GlyphGraphicsItem( FT_GlyphSlot glyph, double size ) ;

	// implementation of pure virtual functions
	QRectF boundingRect() const ;
	
	void paint(
		QPainter 						*painter,
		const QStyleOptionGraphicsItem	*option,
		QWidget							*widget ) ;

private :
	FT_Glyph			m_glyph ;
	FT_Glyph_Metrics	m_metrics ;
	double				m_size ;
} ;

} // end of namespace

#endif // GLYPHGRAPHICSITEM_HH_
