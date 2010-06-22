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

/**	\file	PathObject.hh
    \brief	definition the PathObject class
    \date	Apr 7, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_PATHOBJECT_HH_EADER_INCLUDED__
#define __PDF_PATHOBJECT_HH_EADER_INCLUDED__

#include "GraphicsObject.hh"

#include "QtGraphicsState.hh"

#include <QBrush>
#include <QPainterPath>
#include <QPen>
#include <QRectF>

namespace pdf {

class Path ;

///	brief description
/**	\internal
	The PathObject class represents
*/
class PathObject : public GraphicsObject
{
public :
	explicit PathObject( const Path *path, QGraphicsItem *parent = 0 ) ;

	// virtual functions for QGraphicsItem
	QRectF boundingRect( ) const ;
	void paint(
		QPainter 						*painter,
		const QStyleOptionGraphicsItem	*option,
		QWidget 						*widget ) ; 

	GraphicsState Format( ) const ;

	Graphics* Write( ) const ;

	QString ObjectType( ) const ;

protected :
	bool OnChangeState( const GraphicsState& new_gs ) ;
	
private :
	void CalculateBounding( ) ;
	
	QPen Pen( ) const ;
	QBrush Brush( ) const ;

private :
	QPainterPath	m_path ;
	QtGraphicsState	m_format ;
	
	QRectF	m_bound ;
	
	bool	m_stroke ;
	bool	m_fill ;
} ;

} // end of namespace

#endif // PATHOBJECT_HH_
