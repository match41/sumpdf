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

#include <libpdfdoc.hh>
#include <graphics/Text.hh>
#include <graphics/TextLine.hh>
#include <graphics/TextState.hh>
#include <graphics/Color.hh>
#include <util/Debug.hh>
#include <util/Matrix.hh>

#include <QColor>
#include <QDebug>
#include <QPaintDevice>
#include <QPaintEngine>
#include <QPaintEngineState>
#include <QPainter>
#include <QTextDocument>
#include <QFontInfo>
#include <QFontMetricsF>
#include <QTextBlock>
#include <QTextFragment>
#include <QTextCharFormat>
#include <QTextLayout>

#include <limits>
#include <iostream>

namespace pdf {

namespace {

class PaintEngine : public QPaintEngine
{
public :
	explicit PaintEngine( QGraphicsItem *owner, DocModel *doc )
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
	
	bool end()
	{
		return true ;
	}
	
	void drawPixmap( const QRectF& r, const QPixmap & pm, const QRectF & sr )
	{
	}
	
	void drawPolygon( const QPointF *points, int pointCount, PolygonDrawMode mode )
	{
	}
	
	void drawTextItem( const QPointF& pos, const QTextItem& item )
	{
		Font *f = m_doc->CreatePdfFont( item.font() ) ;

		PDF_ASSERT( f != 0 ) ;

		GraphicsState gs ;
		gs.Text().ChangeFont( QFontInfo(item.font()).pixelSize(), f ) ;
		gs.FillColor( m_color ) ;

		// the way Qt calculate character width is less accurate that us
		// so we can't just make a text line with the whole string.
		// we need to break it down into character and ask Qt the width
		// of each character and advance the position.
		QFontMetrics met(item.font() ) ;
		double offset = 0 ;
		foreach( QChar ch, item.text() )
		{
//			std::auto_ptr<TextLine> line( CreateTextLine( gs,
//				Matrix::Translation( pos.x() + offset, -pos.y() ), 
//				ToWStr( QString(ch) ) ) ) ;

			new TextLineObject( gs,
				Matrix::Translation( pos.x() + offset, -pos.y() ),
				QString(ch),
				m_owner ) ;
			
			offset += met.width(ch) ;
		}
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
	
	QGraphicsItem	*m_owner ;
	DocModel	*m_doc ;
	
	Color		m_color ;
} ;

class PaintDevice : public QPaintDevice
{
public :
	explicit PaintDevice( QGraphicsItem *owner, DocModel *doc )
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
	painter.setRenderHints( QPainter::TextAntialiasing ) ;
	text->drawContents( &painter ) ;
	
/*	double voffset = 0 ;
	
	QTextBlock b = text->firstBlock() ;
	while ( b.isValid() )
	{
		double max_height	= 0 ;
		double hoffset		= 0 ;
		for ( QTextBlock::iterator i = b.begin() ; i != b.end() ; ++i )
		{
			QTextFragment frag = i.fragment() ;
			QTextCharFormat format = frag.charFormat() ;
			Font *f = doc->CreatePdfFont( format.font() ) ;

			GraphicsState gs ;
			gs.Text().ChangeFont( format.font().pointSizeF(), f ) ;
			gs.FillColor( FromQColor( format.foreground().color() ) ) ;
			
			TextLine line( gs,
				Matrix::Translation( hoffset, voffset ), 
				ToWStr( frag.text() ) ) ;

			hoffset += line.Width() ;
			max_height = std::max( max_height, gs.Text().Height() ) ;
			
			new TextLineObject( line, this ) ;
		}

		b = b.next() ;
		voffset -= max_height ;
	}
*/
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

		std::auto_ptr<TextLine> tline = line->GetLine() ;
		tline->SetTransform(
			Matrix::Translation( x(), y() ) * 
			Matrix::Translation( line->x(), line->y() ) *
			tline->Transform() ) ;

		t->AddLine( tline.get() ) ;
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

QString TextObject::ObjectType( ) const
{
	return tr( "Text" ) ;
}

} // end of namespace
