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

#include "graphics/RealText.hh"

#include <vector>

namespace pdf {

class Stream ;
class Token ;
class Object ;
class Graphics ;
class Resources ;

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
	RealText* AddText( ) ;
	void VisitGraphics( GraphicsVisitor *visitor ) ;

	void Add( Graphics *gfx ) ;
	bool IsEmpty( ) const ;

	template <typename InputIt>
	void Load( InputIt first, InputIt last, Resources *res )
	{
		while ( first != last )
			Load( *first++, res ) ;
	}

private :
	void Load( Stream& str, Resources *res ) ;

private :
	Graphics* ProcessCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t 	count,
		Graphics		*gfx,
		Resources 		*res  ) ;

private :
	std::vector<Graphics*> m_gfx ;
} ;

} // end of namespace

#endif // REALCONTENT_HH_
