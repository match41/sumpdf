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

/**	\file	RealText.hh
    \brief	definition the RealText class
    \date	Jan 13, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REALTEXT_HH_EADER_INCLUDED__
#define __PDF_REALTEXT_HH_EADER_INCLUDED__

#include "graphics/Text.hh"

#include "graphics/TextLine.hh"

namespace pdf {

///	brief description
/**	The RealText class represents
*/
class RealText : public Text
{
public :
	RealText( ) ;

	void OnCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t		count,
		Resources		*res ) ;

	void Output( std::ostream& os ) const ;

	iterator begin() ;
	iterator end() ;
	
	const_iterator begin() const ;
	const_iterator end() const ;
	
	std::size_t Count( ) const ;
	
	void AddLine( const TextLine& line ) ;

	void Visit( GraphicsVisitor *visitor ) ;

private :
	std::vector<TextLine>	m_lines ;
} ;

} // end of namespace

#endif // REALTEXT_HH_
