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
#include "graphics/ImageObject.hh"
#include "graphics/PathObject.hh"
#include "graphics/TextLineObject.hh"
#include "graphics/TextObject.hh"
#include "Util.hh"

// libpdfdoc headers
#include <graphics/Path.hh>
#include <graphics/PathSegment.hh>
#include <graphics/Text.hh>
#include <util/Debug.hh>
#include <util/Matrix.hh>

// Qt headers
#include <QGraphicsScene>
#include <QPainterPath>
#include <QDebug>

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
	
//	std::for_each( text->begin(), text->end(),
//		boost::bind( &PageLoader::LoadTextLine, this, g, _1 ) ) ;
	for ( std::size_t i = 0 ; i < text->Count() ; i++ )
		LoadTextLine( g, text->At(i) ) ;
	
	m_scene->addItem( g ) ;
}

void PageLoader::LoadTextLine( QGraphicsItem *parent, const TextLine *line )
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
	PDF_ASSERT( m_scene != 0 ) ;
	m_scene->addItem( new ImageObject( img ) ) ;
}

void PageLoader::VisitGraphicsLink( ExtGraphicsLink<GraphicsGroup> *g )
{
}

void PageLoader::SetTransform( Graphics *gfx, QGraphicsItem  *go )
{
	go->setTransform( ToQtMatrix( gfx->Transform() ) ) ;
}

} // end of namespace
