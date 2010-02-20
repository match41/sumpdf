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

class Graphics ;
class Resources ;

///	brief description
/**	\internal
	The ContentStream class represents
*/
class ContentStream
{
public :
	template <typename InputIt>
	ContentStream( InputIt first, InputIt last, const Resources *res )
		: m_strs( first, last ),
		  m_res( res ),
		  m_current( 0 )
	{
	}

	void Decode( ) ;

	void SwapGfxObj( std::vector<Graphics*>& gfxs ) ;

private :
	struct HandlerMap ;
	
	void Decode( Stream& str ) ;

	void ProcessCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t 	count ) ;

	void OnBT( Object *args, std::size_t count ) ;
	void OnET( Object *args, std::size_t count ) ;
	void Oncm( Object *args, std::size_t count ) ;
	void OnQ( Object *args, std::size_t count ) ;
	void Onq( Object *args, std::size_t count ) ;

private :
	std::vector<Stream>		m_strs ;
	std::vector<Graphics*>	m_gfx ;
	const Resources			*m_res ;

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
} ;

} // end of namespace

#endif // CONTENTSTREAM_HH_
