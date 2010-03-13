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

/**	\file	GlyphGroup.hh
    \brief	definition the GlyphGroup class
    \date	Jan 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GLYPHGROUP_HH_EADER_INCLUDED__
#define __PDF_GLYPHGROUP_HH_EADER_INCLUDED__

#include <QGraphicsItemGroup>
#include <QAbstractTableModel>

#include <graphics/CharVisitor.hh>

#include <graphics/TextLine.hh>

namespace pdf {

class Matrix ;
class GraphicsState ;

///	brief description
/**	The GlyphGroup class represents
*/
class GlyphGroup :
	public QGraphicsItemGroup,
	public QAbstractTableModel,
	private CharVisitor
{
public :
	explicit GlyphGroup( const TextLine& blk, QGraphicsItem *parent = 0 ) ;

	void OnChar(
		wchar_t 			ch,
		double				offset,
		const Glyph			*glyph,
		const TextState&	state ) ; 

	int type( ) const ;

	static const int Type = UserType + 1 ;

	const GraphicsState& Format( ) const ;
	
	TextLine GetLine( ) const ;
	
	QVariant itemChange( GraphicsItemChange change, const QVariant& value ) ;

	//@{
	/// abstract table model members
	int rowCount( const QModelIndex& parent ) const ;
	int columnCount( const QModelIndex& parent ) const ;
	
	QVariant data( const QModelIndex& index, int role ) const ;
	QVariant headerData( int sect, Qt::Orientation ori, int role ) const ;
	//@}

private :
	TextLine		m_line ;
} ;

} // end of namespace

#endif // GLYPHGROUP_HH_
