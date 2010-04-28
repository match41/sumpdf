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

/**	\file	QtGraphicsState.hh
    \brief	definition the QtGraphicsState class
    \date	Apr 18, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_QTGRAPHICSSTATE_HH_EADER_INCLUDED__
#define __PDF_QTGRAPHICSSTATE_HH_EADER_INCLUDED__

#include <graphics/GraphicsState.hh>

class QBrush ;
class QPen ;
class QPainterPathStroker ;

namespace pdf {

class Color ;

///	brief description
/**	\internal
	The QtGraphicsState class represents
*/
class QtGraphicsState
{
public :
	explicit QtGraphicsState( const GraphicsState& gs = GraphicsState() ) ;

	const GraphicsState& Get() const ;

	QPen Pen() const ;
	QBrush Brush( ) const ;

	void InitStroker( QPainterPathStroker& qpps ) const ;

	static QBrush Brush( const Color& color ) ;

private :
	GraphicsState	m_gs ;
} ;

} // end of namespace

#endif // QTGRAPHICSSTATE_HH_
