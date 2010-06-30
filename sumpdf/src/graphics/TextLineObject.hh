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

/**	\file	GlyphGroup.hh
    \brief	definition the GlyphGroup class
    \date	Jan 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GLYPHGROUP_HH_EADER_INCLUDED__
#define __PDF_GLYPHGROUP_HH_EADER_INCLUDED__

#include "GraphicsObject.hh"

#include <QRectF>

#include <graphics/CharVisitor.hh>
#include <graphics/GraphicsState.hh>

#include <memory>

namespace pdf {

class Matrix ;
class GraphicsState ;
class TextLine ;

///	brief description
/**	The GlyphGroup class represents
*/
class TextLineObject :
	public QAbstractGraphicsShapeItem,
	private CharVisitor
{
public :
	explicit TextLineObject( const TextLine *blk, QGraphicsItem *parent = 0 ) ;
	TextLineObject(
		const GraphicsState&	format,
		const Matrix&			transform,
		const QString&			text,
		QGraphicsItem 			*parent = 0 ) ;

	~TextLineObject( ) ;

	void AddChar( wchar_t ch, double offset ) ;
	void AddString( const QString& str ) ;

	GraphicsState Format( ) const ;
	
	std::auto_ptr<TextLine> GetLine( ) const ;
	
	// virtual functions for QGraphicsItem
	QRectF boundingRect( ) const ;
	void paint(
		QPainter 						*painter,
		const QStyleOptionGraphicsItem	*option,
		QWidget 						*widget ) ; 

private :
	void OnChar(
		wchar_t 			ch,
		double				offset,
		const Glyph			*glyph,
		const TextState&	state ) ; 
	
private :
	std::auto_ptr<TextLine>	m_line ;
//	GraphicsState	m_format ;
	QRectF			m_bound ;
} ;

} // end of namespace

#endif // GLYPHGROUP_HH_
