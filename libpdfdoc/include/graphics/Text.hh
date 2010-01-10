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

/**
    \file	Text.hh
    \brief	definition the Text class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXT_HH_EADER_INCLUDED__
#define __PDF_TEXT_HH_EADER_INCLUDED__

#include "Graphics.hh"

#include "TextLine.hh"

#include <vector>

namespace pdf {

/**	\brief	brief description

	The Text class represent a PDF text object. It is the stuff enclosed by
	a BT...ET operators in the content stream of a page. It consists of a number
	of text lines.
*/
class Text : public Graphics
{
public :
	typedef	std::vector<TextLine>::iterator			iterator ;
	typedef	std::vector<TextLine>::const_iterator	const_iterator ;

public :
	Text( ) ;

	void OnCommand(
		const Token& 	cmd,
		Object 			*args,
		std::size_t		count,
		Resources		*res ) ;

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

#endif // TEXT_HH_
