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
    \file	TextLine.hh
    \brief	definition the TextLine class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTLINE_HH_EADER_INCLUDED__
#define __PDF_TEXTLINE_HH_EADER_INCLUDED__

#include "TextBlock.hh"

#include <vector>

namespace pdf {

/**	\brief	brief description

	The TextLine class represent a line of text. Inside the line, the text
	matrix (e.g. position) will not change. A line of text contains a list
	of text blocks. Each block will have a string of characters with the same
	formatting parameters.
*/
class TextLine
{
public :
	TextLine( ) ;

private :
	/// the text matrix
	double	m_mat[6] ;
	
	std::vector<TextBlock>	m_blks ;
} ;

} // end of namespace

#endif // TEXTLINE_HH_
