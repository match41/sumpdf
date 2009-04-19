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
#include <boost/tuple/tuple.hpp>

// stdc++ headers
#include <iosfwd>
#include <vector>

namespace pdf {

namespace TextState
{
	enum Type
	{
		char_space, word_space, scale, leading, render_mode,
		text_rise, none
	} ;
	
	enum { type, arg } ;
	typedef boost::tuple<Type, double>	_ ;
}

namespace TextFont
{
	enum { font, size } ;
	typedef boost::tuple<Name, double> _ ;
}

namespace TextPosition
{
	enum { offx, offy } ;
	typedef boost::tuple<double, double> _ ;
}

namespace TextMatrix
{
	enum { a, b, c, d, e, f } ;
	typedef boost::tuple<double, double, double, double, double, double> _ ;
}

namespace TextString
{
	typedef std::string _ ;
}

namespace TextPosString
{
	typedef boost::variant<std::string, int> Entry ;
	typedef std::vector<Entry>	_ ;
}

struct BeginText {} ;
struct EndText {} ;

} // end of namespace

#endif
