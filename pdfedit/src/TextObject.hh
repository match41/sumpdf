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

/**	\file	TextObject.hh
    \brief	definition the TextObject class
    \date	May 7, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTOBJECT_HH_EADER_INCLUDED__
#define __PDF_TEXTOBJECT_HH_EADER_INCLUDED__

#include "GraphicsObject.hh"

namespace pdf {

///	brief description
/**	\internal
	The TextObject class represents
*/
class TextObject : public GraphicsObject
{
public :
	explicit TextObject( QGraphicsItem *parent = 0 ) ;
	
	// virtual functions for QGraphicsItem
	QRectF boundingRect( ) const ;
	void paint(
		QPainter 						*painter,
		const QStyleOptionGraphicsItem	*option,
		QWidget 						*widget ) ; 

	Graphics* Write( ) const ;

	GraphicsState Format( ) const ;

protected :
	bool OnChangeState( const GraphicsState& new_gs ) ;
} ;

} // end of namespace

#endif // TEXTOBJECT_HH_
