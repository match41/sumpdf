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

/**	\file	ImageObject.cc
	\brief	implementation of the ImageObject class
	\date	May 23, 2010
	\author	Nestal Wan
*/

#include "ImageObject.hh"

#include "Util.hh"

#include <graphics/ColorSpace.hh>
#include <graphics/ExtGraphicsLink.hh>
#include <graphics/GraphicsState.hh>
#include <graphics/Image.hh>

#include <util/Debug.hh>

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPainter>

namespace pdf {

/**	constructor
	
*/
ImageObject::ImageObject(
	const ExtGraphicsLink<Image>	*link,
	QGraphicsItem					*parent )
	: GraphicsObject( parent )
	, m_child( 0 )
	, m_format( link->GetState() )
{
	PDF_ASSERT( link != 0 ) ;
	const Image *img = link->Get() ;
	PDF_ASSERT( img != 0 ) ;
	m_child = new QGraphicsPixmapItem(
		QPixmap::fromImage( ToQImage(img).mirrored() ),
		this ) ;
	
	// this is ugly. need to clean it up
	QTransform m = ToQtMatrix( link->Transform() ) ;
	m.scale( 1.0/img->Width(), 1.0/img->Height() ) ;
	setTransform( m ) ;
}

// TODO: implement proper construction from external
ImageObject::ImageObject( const QImage& image, QGraphicsItem *parent )
	: GraphicsObject( parent )
	, m_child( 0 )
{
}

QImage ImageObject::ToQImage( const Image *img )
{
	if ( img->IsJpeg() )
	{
		QImage result ;
		result.loadFromData( img->Pixels(), img->ByteCount() ) ;
		return result ;
	}
	else
	{
		QImage tmp( img->Pixels(), img->Width(), img->Height(), img->Width(),
			QImage::Format_Indexed8 ) ;
			
		if ( img->Space() != 0 )
		{
			tmp.setColorCount( img->Space()->ColorCount() ) ;
			QVector<QRgb> cmap ;
			for ( std::size_t j = 0 ; j < img->Space()->ColorCount() ; ++j )
				cmap.push_back( ToQColor(img->Space()->Lookup(j)).rgb() ) ;
				
			tmp.setColorTable(cmap) ;
		}
		
		return tmp ;
	}
}

GraphicsState ImageObject::Format() const
{
	return m_format.Get() ;
}

Graphics* ImageObject::Write( ) const
{
	return 0 ;
}

QRectF ImageObject::boundingRect( ) const
{
	return m_child->boundingRect( ) ;
}

void ImageObject::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget 						*widget ) 
{
	GraphicsObject::paint( painter, option, widget ) ;
}

bool ImageObject::OnChangeState( const GraphicsState& new_gs )
{
	return false ;
}

QString ImageObject::ObjectType( ) const
{
	return tr( "Image" ) ;
}

} // end of namespace
