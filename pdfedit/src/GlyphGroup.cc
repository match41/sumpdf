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

#include <util/Debug.hh>
#include <util/Matrix.hh>

#include <QDebug>

namespace pdf {

/**	constructor
*/
GlyphGroup::GlyphGroup( const TextLine& blk, QGraphicsItem *parent )
	: QGraphicsItemGroup( parent ),
	  m_text( QString::fromStdWString(blk.Text()) ),
	  m_state( blk.Format() )
{
	blk.VisitChars( this ) ;
	// setup flags
	setFlags( ItemIsSelectable | ItemIsMovable ) ;
}

void GlyphGroup::OnChar(
	wchar_t 		ch,
	const Matrix&	m,
	const Glyph&	glyph,
	double			scale_factor ) 
{
	GlyphGraphicsItem *item = new GlyphGraphicsItem( glyph ) ;

	// scale font by their font size
	item->setTransform( ToQtMatrix( m ) ) ;
	item->scale( scale_factor, scale_factor ) ;

	addToGroup( item ) ;
}

int GlyphGroup::type( ) const
{
	return Type ;
}

const TextState& GlyphGroup::Format( ) const
{
	return m_state ;
}

} // end of namespace
