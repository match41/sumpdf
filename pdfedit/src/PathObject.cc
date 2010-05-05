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

#include "Util.hh"

// libpdfdoc headers
#include <graphics/Path.hh>
#include <graphics/PathSegment.hh>
#include <util/Debug.hh>
#include <util/Matrix.hh>
#include <util/Util.hh>

// Qt headers
#include <QPainter>
#include <QPainterPathStroker>

#include <QDebug>

namespace pdf {

/**	constructor
	
	\param	path	the path object read from the PDF file. It may be
					deleted by the caller after this call so this class
					cannot store this pointer.
*/
PathObject::PathObject( const Path *path, QGraphicsItem *parent )
	: GraphicsObject( parent )
	, m_path( QPointF(0, 0) )
	, m_format( path->GetState() )
	, m_stroke( path->IsStroke( ) )
	, m_fill( path->IsFill() )
{
	PDF_ASSERT( path != 0 ) ;

	m_path.setFillRule( path->FillRule() == Path::winding
		? Qt::WindingFill : Qt::OddEvenFill ) ;

	for ( std::size_t i = 0 ; i < path->Count() ; ++i )
	{
		PathSegment seg = path->Segment(i) ;
		switch ( seg.GetOp() )
		{
			case PathSegment::move : m_path.moveTo( seg[0], seg[1] ) ; break ;
			case PathSegment::line : m_path.lineTo( seg[0], seg[1] ) ; break ;
			
			case PathSegment::cubic123 :
				m_path.cubicTo(seg[0], seg[1], seg[2], seg[3], seg[4], seg[5]) ;
				break ;
			
			case PathSegment::cubic23 :
				m_path.cubicTo(
					m_path.currentPosition(),
					QPointF(seg[0], seg[1]),
					QPointF(seg[2], seg[3]) ) ;
				break ;

			case PathSegment::cubic13 :
				m_path.cubicTo(
					QPointF(seg[0], seg[1]),
					QPointF(seg[2], seg[3]),
					QPointF(seg[2], seg[3]) ) ;
				break ;
			
			case PathSegment::close: m_path.closeSubpath( ) ; break ;
			default : break ;
		}
	}
	
	setTransform( ToQtMatrix( path->Transform() ) ) ;
	
	CalculateBounding( ) ;
}

void PathObject::CalculateBounding( )
{
	// use stroker path to calculate bounding rectangle
	QPainterPathStroker sk ;
	m_format.InitStroker( sk ) ;
	
	QPainterPath sk_path = sk.createStroke( m_path ) ;
	m_bound = sk_path.boundingRect() ;
}

QRectF PathObject::boundingRect( ) const
{
	return m_bound ;
}

void PathObject::paint(
	QPainter 						*painter,
	const QStyleOptionGraphicsItem	*option,
	QWidget 						*widget ) 
{
	PDF_ASSERT( painter != 0 ) ;

	GraphicsObject::paint( painter, option, widget ) ;

	// set colors
	painter->setBrush( Brush() ) ;
	painter->setPen( Pen() ) ;

	// actually draw the path
	painter->drawPath( m_path ) ;
}

GraphicsState PathObject::Format( ) const
{
	return m_format.Get() ;
}

QPen PathObject::Pen( ) const
{
	return m_stroke ? m_format.Pen() : QPen( QBrush(Qt::NoBrush), 0 ) ;
}

QBrush PathObject::Brush( ) const
{
	return m_fill ? m_format.Brush() : QBrush( Qt::NoBrush ) ;
}

Graphics* PathObject::Write( ) const
{
	Path *path = CreatePath( m_format.Get() ) ;
	path->SetStyle(
		m_fill,
		m_stroke,
		m_path.fillRule() == Qt::WindingFill
			? Path::winding : Path::odd_even ) ;

	path->Transform(
		FromQtMatrix( transform() ) * Matrix::Translation( x(), y() ) ) ;
	
	std::vector<double> cubic ;
	for ( int i = 0 ; i < m_path.elementCount() ; ++i )
	{
		QPainterPath::Element e = m_path.elementAt(i) ;
		switch ( e.type )
		{
			case QPainterPath::MoveToElement :
			{
				double pt[] = { e.x, e.y } ;
				path->AddSegment( PathSegment( PathSegment::move, pt ) ) ;
				break ;
			}
			case QPainterPath::LineToElement :
			{
				double pt[] = { e.x, e.y } ;
				path->AddSegment( PathSegment( PathSegment::line, pt ) ) ;
				break ;
			}
			case QPainterPath::CurveToElement :
			case QPainterPath::CurveToDataElement :
				cubic.push_back( e.x ) ;
				cubic.push_back( e.y ) ;
				
				if ( cubic.size() ==
					PathSegment::ArgCount( PathSegment::cubic123 ) )
				{
					path->AddSegment( PathSegment(
						PathSegment::cubic123, &cubic[0] ) ) ;
					cubic.clear() ;
				}
				
				break ;

			default :
				break ;
		}
	}
	return path ;
}

bool PathObject::OnChangeState( const GraphicsState& new_gs )
{
	return false ;
}

} // end of namespace
