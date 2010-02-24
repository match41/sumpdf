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

#include "GlyphGroup.hh"

#include "GlyphGraphicsItem.hh"
#include "Util.hh"

#include <graphics/TextLine.hh>

#include "Util.hh"

#include <util/Debug.hh>
#include <util/Matrix.hh>

#include <QDebug>

namespace pdf {

/**	constructor
*/
GlyphGroup::GlyphGroup( const TextLine& blk, QGraphicsItem *parent )
	: QGraphicsItemGroup( parent )
	, m_line( blk )
//	, m_text( FromWStr(blk.Text()) )
//	, m_state( blk.Format() )
//	, m_pos( blk.XPos(), blk.YPos() )
{
	m_line.VisitChars( this ) ;
	
	// setup flags
	setFlags( ItemIsSelectable | ItemIsMovable ) ;

	QTransform t = ToQtMatrix( m_line.Transform() ) ;
	setTransform( t ) ;
}

void GlyphGroup::OnChar(
	wchar_t 			ch,
	double				offset,
	const Glyph			*glyph,
	const TextState&	state ) 
{
	GlyphGraphicsItem *item = new GlyphGraphicsItem( glyph ) ;

	// set offset
	item->translate( m_line.XPos() + offset, m_line.YPos() ) ;
	
	// scale font by their font size
	item->scale( state.ScaleFactor(), state.ScaleFactor() ) ;

	addToGroup( item ) ;
}

int GlyphGroup::type( ) const
{
	return Type ;
}

const GraphicsState& GlyphGroup::Format( ) const
{
	return m_line.Format() ;
}

/*
QString GlyphGroup::Text( ) const
{
	return m_text ;
}
*/

TextLine GlyphGroup::GetLine( ) const
{
	TextLine line( m_line ) ;
//	line.XPos( m_line.XPos() + x() ) ;
//	line.YPos( m_line.YPos() + y() ) ;
	return line ; 
}

} // end of namespace
