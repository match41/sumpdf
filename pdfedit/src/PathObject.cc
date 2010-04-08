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

/**	\file	PathObject.cc
	\brief	implementation of the PathObject class
	\date	Apr 7, 2010
	\author	Nestal Wan
*/

#include "PathObject.hh"

// libpdfdoc headers
#include <graphics/Path.hh>
#include <graphics/PathSegment.hh>
#include <util/Debug.hh>

// Qt headers
#include <QPainter>

namespace pdf {

/**	constructor
	
*/
PathObject::PathObject( const Path *path, QGraphicsItem *parent )
	: GraphicsObject( parent )
	, m_path( QPointF(0, 0) )
	, m_format( path->GetState() )
{
	for ( std::size_t i = 0 ; i < path->Count() ; ++i )
	{
		PathSegment seg = path->Segment(i) ;
		switch ( seg.GetOp() )
		{
			case PathSegment::move : m_path.moveTo( seg[0], seg[1] ) ; break ;
			case PathSegment::line : m_path.lineTo( seg[0], seg[1] ) ; break ;
			case PathSegment::close: m_path.closeSubpath( ) ; break ;
			default : break ;
		}
	}
}

QRectF PathObject::boundingRect( ) const
{
	return m_path.boundingRect() ;
}

void PathObject::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget 						*widget ) 
{
	PDF_ASSERT( painter != 0 ) ;

	GraphicsObject::paint( painter, option, widget ) ;
	painter->drawPath( m_path ) ;
}

GraphicsState PathObject::Format( ) const
{
	return m_format ;
}

} // end of namespace
