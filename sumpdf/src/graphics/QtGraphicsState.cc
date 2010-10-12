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

/**	\file	QtGraphicsState.cc
	\brief	implementation of the QtGraphicsState class
	\date	Apr 18, 2010
	\author	Nestal Wan
*/

#include "QtGraphicsState.hh"

#include "Util.hh"

#include <graphics/Color.hh>
#include <util/CArray.hh>
#include <util/Debug.hh>

#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPainterPathStroker>

namespace pdf {

/**	constructor
	
*/
QtGraphicsState::QtGraphicsState( const GraphicsState& gs )
	: m_gs( gs )
{
}

const GraphicsState& QtGraphicsState::Get() const
{
	return m_gs ;
}

QPen QtGraphicsState::Pen() const
{
	Qt::PenJoinStyle jmap[] = { Qt::MiterJoin, Qt::RoundJoin, Qt::BevelJoin } ;
	PDF_ASSERT( static_cast<std::size_t>(m_gs.LineJoin()) < Count(jmap) ) ;

	// fill colour
	QPen p( ToQColor( m_gs.StrokeColor() ) ) ;
	p.setWidthF( m_gs.LineWidth( ) ) ;
	p.setJoinStyle( jmap[m_gs.LineJoin()] ) ;
	p.setMiterLimit( m_gs.MiterLimit() ) ;
	return p ;
}

QBrush QtGraphicsState::Brush( ) const
{
	return Brush( m_gs.FillColor() ) ;
}

QBrush QtGraphicsState::Brush( const Color& color )
{
	return QBrush( ToQColor( color ) ) ;
}

void QtGraphicsState::InitStroker( QPainterPathStroker& qpps ) const
{
	QPen pen = Pen() ;

	qpps.setWidth( pen.widthF() ) ;
	qpps.setJoinStyle( pen.joinStyle() ) ;
	qpps.setMiterLimit( pen.miterLimit() ) ;
}

} // end of namespace
