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
    \file	TextBlock.hh
    \brief	definition the TextBlock class
    \date	Jan 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TEXTBLOCK_HH_EADER_INCLUDED__
#define __PDF_TEXTBLOCK_HH_EADER_INCLUDED__

#include "TextFormat.hh"
#include <vector>

namespace pdf {

/**	\brief	brief description

	The TextBlock class represent a string of characters with the same
	format.
*/
class TextBlock
{
public :
	TextBlock( ) ;

private :
	std::vector<unsigned short>	m_chars ;
	TextFormat	m_format ;
} ;

} // end of namespace

#endif // TEXTBLOCK_HH_
