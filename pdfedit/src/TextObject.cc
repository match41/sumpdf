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

/**	\file	TextObject.cc
	\brief	implementation of the TextObject class
	\date	May 7, 2010
	\author	Nestal Wan
*/

#include "TextObject.hh"

#include "TextLineObject.hh"

#include <graphics/Text.hh>
#include <util/Debug.hh>

namespace pdf {

/**	constructor
	
*/
TextObject::TextObject( QGraphicsItem *parent )
	: GraphicsObject( parent )
{
}

// virtual functions for QGraphicsItem
QRectF TextObject::boundingRect( ) const
{
	return childrenBoundingRect( ) ;
}

void TextObject::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget 						*widget )
{
	GraphicsObject::paint( painter, option, widget ) ;
}

bool TextObject::OnChangeState( const GraphicsState& new_gs )
{
	return false ;
}

Graphics* TextObject::Write( ) const
{
	Text *t = CreateText( GraphicsState() ) ;
	
	foreach ( QGraphicsItem *item, childItems() )
	{
		TextLineObject *line = dynamic_cast<TextLineObject*>( item ) ;
		PDF_ASSERT( line->Format().FontFace() != 0 ) ;
		
		t->AddLine( line->GetLine( ) ) ;
	}
	
	return t ;
}

GraphicsState TextObject::Format( ) const
{
	foreach ( QGraphicsItem *item, childItems() )
	{
		TextLineObject *line = dynamic_cast<TextLineObject*>( item ) ;
		return line->Format( ) ;
	}
	return GraphicsState() ;
}


} // end of namespace
