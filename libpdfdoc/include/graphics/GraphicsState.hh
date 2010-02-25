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

/**	\file	GraphicsState.hh
    \brief	definition the GraphicsState class
    \date	Feb 16, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_GRAPHICSSTATE_HH_EADER_INCLUDED__
#define __PDF_GRAPHICSSTATE_HH_EADER_INCLUDED__

#include "Colour.hh"
#include "TextState.hh"

#include "util/Matrix.hh"

#include <iosfwd>
#include <map>

namespace pdf {

class ContentOp ;
class Resources ;
class Token ;

///	The PDF graphics state.
/**	\internal
	The graphics state is a set of graphics control parameters. These
	parameters define the way that the graphics operators execute. 
*/
class GraphicsState
{
public :
	explicit GraphicsState( const TextState& ts = TextState() ) ;

	const TextState& GetTextState() const ;
	TextState& GetTextState() ;

	Font* GetFont( ) const ;

	std::ostream& Print(
		std::ostream&			os,
		const Resources			*res,
		const GraphicsState&	prev = GraphicsState() ) const ;

	/// Handle PDF content operators.
	bool OnCommand( ContentOp& op, const Resources *res ) ;

	static bool IsGSCommand( const Token& cmd ) ;

	bool operator==( const GraphicsState& gs ) const ;
	bool operator!=( const GraphicsState& gs ) const ;

	friend std::ostream& operator<<(std::ostream& os, const GraphicsState& gs) ;

	void LineWidth( double value ) ;
	double LineWidth( ) const ;

	void LineCap( int value ) ;
	int LineCap( ) const ;
	
	void LineJoin( int value ) ;
	int LineJoin( ) const ;
	
	void MiterLimit( double value ) ;
	double MiterLimit( ) const ;

private :
	struct HandlerMap ;

	// text state command handlers
	bool OnTf( ContentOp& op, const Resources *res ) ;
	bool OnTL( ContentOp& op, const Resources *res ) ;

private :
	TextState	m_text ;
	
	Colour		m_colour ;
	
	double		m_line_width ;
	int			m_line_cap ;
	int			m_line_join ;
	double		m_miter_limit ;
} ;

} // end of namespace

#endif // GRAPHICSSTATE_HH_
