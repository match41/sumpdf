/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
 ***************************************************************************/

/*!
	\file	TextOps.hh
	\brief	definition the TextSegment class
	\date	Mon Mar 2 2009
	\author	Nestal Wan
*/

#ifndef __PDF_TEXT_OPS_HEADER_INCLUDED__
#define __PDF_TEXT_OPS_HEADER_INCLUDED__

#include <boost/variant.hpp>
#include <vector>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
struct TextState
{
	enum Type
	{
		// 
		char_space, word_space, scale, leading, font_size, render_mode,
		text_rise
	} ;

	Type	m_type ;
	double	m_arg ;
} ;

struct TextPosition
{
	double	m_offx, m_offy ;
} ;

struct TextMatrix
{
	double a, b, c, d, e, f ;
} ;

struct TextString
{
	std::string	m_text ;
} ;

struct TextPosString
{
	typedef boost::variant<std::string, int> Entry ;
	std::vector<Entry> m_entries ;
} ;

struct BeginText {} ;
struct EndText {} ;

} // end of namespace

#endif
