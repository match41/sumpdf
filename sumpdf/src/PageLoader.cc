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

/**	\file	PageLoader.cc
	\brief	implementation of the PageLoader class
	\date	Mar 26, 2010
	\author	Nestal Wan
*/

#include "PageLoader.hh"

// local headers
#include "TextLineObject.hh"
#include "TextObject.hh"
#include "PathObject.hh"
#include "Util.hh"

// libpdfdoc headers
#include <graphics/ExtGraphicsLink.hh>
#include <graphics/ColorSpace.hh>
#include <graphics/Image.hh>
#include <graphics/Path.hh>
#include <graphics/PathSegment.hh>
#include <graphics/Text.hh>
#include <util/Debug.hh>

// Qt headers
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QPainterPath>
#include <QDebug>
#include <QGraphicsPixmapItem>

// boost headers
#include <boost/bind.hpp>

namespace pdf {

/**	constructor
	
*/
PageLoader::PageLoader( QGraphicsScene *scene )
	: m_scene( scene )
{
	PDF_ASSERT( m_scene != 0 ) ;
}

void PageLoader::VisitText( Text *text )
{
	PDF_ASSERT( text != 0 ) ;

	TextObject *g = new TextObject ;
	g->setTransform( ToQtMatrix( text->Transform() ) ) ;
	SetTransform( text, g ) ;
	
	std::for_each( text->begin(), text->end(),
		boost::bind( &PageLoader::LoadTextLine, this, g, _1 ) ) ;
	
	m_scene->addItem( g ) ;
}

void PageLoader::LoadTextLine( QGraphicsItem *parent, const TextLine& line )
{
	PDF_ASSERT( m_scene != 0 ) ;
	
	new TextLineObject( line, parent ) ;
}

void PageLoader::VisitGraphics( Graphics *gfx )
{
}

void PageLoader::VisitPath( Path *path )
{
	PDF_ASSERT( m_scene != 0 ) ;
	
	PathObject *p = new PathObject( path ) ;
	SetTransform( path, p ) ;

	m_scene->addItem( p ) ;
}

void PageLoader::VisitGraphicsLink( ExtGraphicsLink<Image> *img )
{
	const Image *i = img->Get() ;
	
	QImage pic ;
	
	if ( !i->IsJpeg() )
	{
		QImage qimg( i->Pixels(), i->Width(), i->Height(), i->Width(), QImage::Format_Indexed8 ) ;
		qimg.setColorCount( i->Space()->ColorCount() ) ;
		QVector<QRgb> cmap ;
		for ( std::size_t j = 0 ; j < i->Space()->ColorCount() ; ++j )
			cmap.push_back( ToQColor(i->Space()->Lookup(j)).rgb() ) ;
			
		qimg.setColorTable(cmap) ;
		
		pic = qimg ;
	}
	else
		pic.loadFromData( i->Pixels(), i->ByteCount() ) ;
	
	QGraphicsPixmapItem *p = m_scene->addPixmap( QPixmap::fromImage(pic) ) ;
	
	// this is ugly. need to clean it up
	QTransform m = ToQtMatrix( img->Transform() ) ;
	m.scale( 1.0/i->Width(), 1.0/i->Height() ) ;
	m.translate( 0, 0.5*i->Height() ) ;
	m.scale( 1.0, -1.0 ) ;
	m.translate( 0, -0.5*i->Height() ) ;
	
	p->setTransform( m ) ;
}

void PageLoader::VisitGraphicsLink( ExtGraphicsLink<GraphicsGroup> *g )
{
}

void PageLoader::SetTransform( Graphics *gfx, QGraphicsItem  *go )
{
	go->setTransform( ToQtMatrix( gfx->Transform() ) ) ;
}

} // end of namespace
