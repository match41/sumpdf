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

/**	\file	ImageObject.hh
    \brief	definition the ImageObject class
    \date	May 23, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_IMAGEOBJECT_HEADER_INCLUDED__
#define __PDF_IMAGEOBJECT_HEADER_INCLUDED__

#include "GraphicsObject.hh"

#include "QtGraphicsState.hh"

class QGraphicsPixmapItem ;
class QImage ;

namespace pdf {

class Image ;
template <typename T> class ExtGraphicsLink ;

///	brief description
/**	\internal
	The ImageObject class represents
*/
class ImageObject : public GraphicsObject
{
public :
	explicit ImageObject( const ExtGraphicsLink<Image> *link,
		QGraphicsItem *parent = 0 ) ;

	GraphicsState Format() const ;
	Graphics* Write( ) const ;

	QRectF boundingRect( ) const ;
	void paint(
		QPainter 						*painter,
		const QStyleOptionGraphicsItem	*option,
		QWidget 						*widget ) ; 

	QString ObjectType( ) const ;

protected :
	bool OnChangeState( const GraphicsState& new_gs ) ;

private :
	QImage ToQImage( const Image *img ) ;

private :
	QGraphicsPixmapItem	*m_child ;
	QtGraphicsState		m_format ;
} ;

} // end of namespace

#endif // IMAGEOBJECT_HH_
