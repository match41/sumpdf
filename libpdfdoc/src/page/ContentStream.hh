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

/**	\file	ContentStream.hh
    \brief	definition the ContentStream class
    \date	Feb 20, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_CONTENTSTREAM_HH_EADER_INCLUDED__
#define __PDF_CONTENTSTREAM_HH_EADER_INCLUDED__

#include "graphics/GraphicsState.hh"
#include "stream/Stream.hh"
#include "util/Matrix.hh"

#include <stack>
#include <vector>

namespace pdf {

class ContentOp ;
class Graphics ;
class GraphicsVisitor ;
class Image ;
class ResourcesDict ;

///	brief description
/**	\internal
	The ContentStream class represents
*/
class ContentStream
{
public :
	template <typename InputIt>
	ContentStream( InputIt first, InputIt last,
		const ResourcesDict	*res,
		GraphicsVisitor		*visitor )
		: m_strs( first, last )
		, m_visitor( visitor )
		, m_res( res )
		, m_current( 0 )
	{
	}

	void Decode( ) ;

private :
	struct HandlerMap ;
	
	void Decode( Stream& str ) ;

	void ProcessCommand( ContentOp& op, std::istream& is ) ;

	void OnBT( ContentOp& op, std::istream& is ) ;
	void OnEndObject( ContentOp& op, std::istream& is ) ;
	void Oncm( ContentOp& op, std::istream& is ) ;
	void OnQ( ContentOp& op, std::istream& is ) ;
	void Onq( ContentOp& op, std::istream& is ) ;
	void Onm( ContentOp& op, std::istream& is ) ;
	void OnPaintPath( ContentOp& op, std::istream& is ) ;
	void OnInlineImage( ContentOp& op, std::istream& is ) ;

private :
	std::vector<Stream>		m_strs ;
	GraphicsVisitor 		*m_visitor ; 
	const ResourcesDict		*m_res ;

	/// all graphics states
	struct State
	{
		GraphicsState	gs ;
		Matrix			ctm ;
	} m_state ;

	//@{
	/// Context information for decoding the graphics objects
	Graphics			*m_current ;
	std::stack<State>	m_state_stack ;
	//@}
	
	// inline images
	std::vector<Image*>	m_inline_imgs ;
} ;

} // end of namespace

#endif // CONTENTSTREAM_HH_
