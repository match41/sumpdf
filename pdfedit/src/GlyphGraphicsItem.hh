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

#include <font/Glyph.hh>

// freetype headers
//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include FT_GLYPH_H

class QPainterPath ;
class QPointF ;

namespace pdf {

class Glyph ;

///	brief description
/**	The GlyphGraphicsItem class represents
*/
class GlyphGraphicsItem : public QGraphicsPathItem
{
public :
	explicit GlyphGraphicsItem( const Glyph& glyph ) ;

private :
/*	static int MoveTo( const FT_Vector* to, void *user ) ;
	int MoveTo( const FT_Vector* to, QPainterPath *p ) ;
	static int LineTo( const FT_Vector* to, void *user ) ;
	int LineTo( const FT_Vector* to, QPainterPath *p ) ;
	static int QuadTo(
		const FT_Vector	*control,
		const FT_Vector	*to,
		void 			*user ) ;
	int QuadTo(
		const FT_Vector* control,
		const FT_Vector* to,
		QPainterPath	 *p ) ;
	static int CubicTo(
		const FT_Vector	*control1,
		const FT_Vector	*control2,
		const FT_Vector	*to,
		void 			*user ) ;
	int CubicTo(
		const FT_Vector	*control1,
		const FT_Vector	*control2,
		const FT_Vector	*to,
		QPainterPath	*p ) ;
*/
	struct Render ;

//	QPointF Transform( const FT_Vector *p ) const ;

private :
//	FT_Glyph_Metrics	m_metrics ;
	Glyph	m_glyph ;
} ;

} // end of namespace

#endif // GLYPHGRAPHICSITEM_HH_
