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

#include "DocModel.hh"
#include "TextLineObject.hh"
#include "Util.hh"

#include <graphics/Text.hh>
#include <graphics/TextLine.hh>
#include <graphics/TextState.hh>
#include <graphics/Color.hh>
#include <util/Debug.hh>

#include <QColor>
#include <QDebug>
#include <QPaintDevice>
#include <QPaintEngine>
#include <QPaintEngineState>
#include <QPainter>
#include <QTextDocument>
#include <QFontInfo>
#include <QFontMetrics>

#include <limits>
#include <iostream>

namespace pdf {

namespace {

class PaintEngine : public QPaintEngine
{
public :
	explicit PaintEngine( TextObject *owner, DocModel *doc )
		: m_owner( owner )
		, m_doc( doc )
	{
		PDF_ASSERT( owner != 0 ) ;
		PDF_ASSERT( doc != 0 ) ;
	}

	bool begin( QPaintDevice * pdev )
	{
		return true ;
	}
	
	void drawPixmap( const QRectF & r, const QPixmap & pm, const QRectF & sr )
	{
	}
	
	bool end()
	{
		return true ;
	}
	
	void drawTextItem( const QPointF& pos, const QTextItem& item )
	{
		Font *f = m_doc->CreateFont( item.font() ) ;

		PDF_ASSERT( f != 0 ) ;

		GraphicsState gs ;
		gs.Text().ChangeFont( QFontInfo(item.font()).pixelSize(), f ) ;
		gs.FillColor( m_color ) ;
		TextLine line( gs,
			Matrix::Translation( pos.x(), pos.y() ), 
			ToWStr( item.text() ) ) ;

qDebug() << "draw text " << item.text() << " at " << pos 
<< " actual width: " << line.Width() << " point size = " << item.font().pointSizeF()
<< " " << QFontInfo(item.font()).pixelSize() ;

QFontMetrics m(item.font() ) ;
qDebug() << "rect = " << m.width(item.text()) ;

		new TextLineObject( line, m_owner ) ;
	}
	
	Type type() const
	{
		return QPaintEngine::User ;
	}
	
	void updateState( const QPaintEngineState& state )
	{
		if ( state.state() | QPaintEngine::DirtyPen )
			m_color = FromQColor( state.pen().color() ) ;
	}
	
	TextObject	*m_owner ;
	DocModel	*m_doc ;
	
	Color		m_color ;
} ;

class PaintDevice : public QPaintDevice
{
public :
	explicit PaintDevice( TextObject *owner, DocModel *doc )
		: m_engine( new PaintEngine( owner, doc ) )
	{
	}

	QPaintEngine* paintEngine( ) const
	{
		return m_engine.get() ;
	}

protected :
	int metric( PaintDeviceMetric metric ) const
	{
		switch (metric)
		{
		case PdmWidth:		return 500 ;
		case PdmHeight:		return 500 ;
		case PdmWidthMM:	return static_cast<int>(500/72.0 * 2.54) ;
		case PdmHeightMM:	return static_cast<int>(500/72.0 * 2.54) ;
		case PdmNumColors:	return std::numeric_limits<int>::max() ;
		case PdmDepth:		return 24 ;
		case PdmDpiX:
		case PdmDpiY:
		case PdmPhysicalDpiX:
		case PdmPhysicalDpiY:	return 72 ;
		
		default :		return 0 ;
		}
	}

private :
	const std::auto_ptr<PaintEngine>	m_engine ;
} ;

}

/**	constructor
	
*/
TextObject::TextObject( QGraphicsItem *parent )
	: GraphicsObject( parent )
{
}

TextObject::TextObject( QTextDocument *text, DocModel *doc,
	QGraphicsItem *parent )
	: GraphicsObject( parent )
{
	PDF_ASSERT( text != 0 ) ;

	PaintDevice dev( this, doc ) ;
	QPainter painter( &dev ) ;
	text->drawContents( &painter ) ;
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

		TextLine tline( line->GetLine() ) ;
		tline.SetTransform(
			Matrix::Translation( x(), y() ) * 
			Matrix::Translation( line->x(), line->y() ) *
			tline.Transform() ) ;

		t->AddLine( tline ) ;
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
