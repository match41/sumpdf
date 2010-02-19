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

/**	\file	RealContent.hh
    \brief	definition the RealContent class
    \date	Jan 14, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALCONTENT_HH_EADER_INCLUDED__
#define __PDF_REALCONTENT_HH_EADER_INCLUDED__

#include "page/PageContent.hh"

#include <vector>

namespace pdf {

class Graphics ;
class Object ;
class Resources ;
class Stream ;
class Text ;
class GraphicsState ;
class Token ;

///	brief description
/**	The RealContent class represents
*/
class RealContent : public PageContent 
{
public :
	RealContent( ) ;

	// operations
	std::size_t Count( ) const ;
	const Graphics* Item( std::size_t idx ) const ;
	Text* AddText( const TextState& ts ) ;
	Text* AddText( double size, Font *font ) ;
	void VisitGraphics( GraphicsVisitor *visitor ) ;

	void Add( Graphics *gfx ) ;
	bool IsEmpty( ) const ;

	template <typename InputIt>
	void Load( InputIt first, InputIt last, Resources *res )
	{
		while ( first != last )
			Load( *first++, res ) ;
	}

	void Write( Stream& str, const Resources *res ) const ;
	
	void Clear( ) ;

private :
	void Load( Stream& str, Resources *res ) ;

private :
	/// This enum denotes the state of decoding graphics objects.
	enum OperatorState
	{
		/// In the page description level, all graphics states operators
		/// are allowed.
		page_description_level,
		
		/// Inside a text object only general graphics states, colour, text
		/// states and text positioning and showing operators are allowed.
		text_object,
		
		/// Only path construction operators are allowed in path objects.
		path_object,
		
		
		clipping_path_object,
		inline_image_object,
	} ;

	Graphics* ProcessCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t 	count,
		GraphicsState&	tstate,
		Graphics		*gfx,
		Resources 		*res ) ;

private :
	std::vector<Graphics*> m_gfx ;
} ;

} // end of namespace

#endif // REALCONTENT_HH_
