/***************************************************************************\
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
\***************************************************************************/

/*!
	\file	TextOps.hh
	\brief	definition the TextSegment class
	\date	Mon Mar 2 2009
	\author	Nestal Wan
*/

#ifndef __PDF_TEXT_OPS_HEADER_INCLUDED__
#define __PDF_TEXT_OPS_HEADER_INCLUDED__

#include "core/Name.hh"

#include <boost/variant.hpp>
#include <boost/typeof/typeof.hpp>

// stdc++ headers
#include <iosfwd>
#include <vector>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
struct TextState
{
	enum Type
	{
		char_space, word_space, scale, leading, render_mode,
		text_rise
	} ;

	typedef Type	Arg1 ;
	typedef double	Arg2 ;

	Type	m_type ;
	double	m_arg ;
} ;

std::ostream& operator<<( std::ostream& os, const TextState& ts ) ;

struct TextFont
{
	typedef Name	Arg1 ;
	typedef double	Arg2 ;
	
	Name	font ;
	double	size ;
} ;

struct TextPosition
{
	typedef double	Arg1 ;
	typedef double	Arg2 ;
	
	double	m_offx, m_offy ;
} ;

struct TextMatrix
{
	typedef double	Arg1 ;
	typedef double	Arg2 ;
	typedef double	Arg3 ;
	typedef double	Arg4 ;
	typedef double	Arg5 ;
	typedef double	Arg6 ;

	double a, b, c, d, e, f ;
} ;

struct TextString
{
	typedef std::string	Arg1 ;

	std::string	m_text ;
} ;

struct TextPosString
{
	typedef boost::variant<std::string, int> Entry ;
	typedef std::vector<Entry>	Arg1 ;
	std::vector<Entry> m_entries ;
} ;

struct BeginText {} ;
struct EndText {} ;

} // end of namespace

#endif
