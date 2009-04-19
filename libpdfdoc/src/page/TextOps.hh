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

#define MAKE_TYPE( name ) \
	namespace name \
	{ \
		typedef boost::tuple<> _ ; \
	}
	
#define MAKE_ONE_TYPE( name, type ) \
	namespace name \
	{ \
		typedef boost::tuple<type> _ ; \
	}

#define MAKE_TWO_TYPE( name, type1, tag1, type2, tag2 ) \
	namespace name \
	{ \
		enum { tag1, tag2 } ; \
		typedef boost::tuple<type1, type2> _ ; \
	}

MAKE_ONE_TYPE( CharSpace, double ) ;
MAKE_ONE_TYPE( WordSpace, double ) ;
MAKE_ONE_TYPE( TextScale, double ) ;
MAKE_ONE_TYPE( TextLeading, double ) ;
MAKE_ONE_TYPE( TextRender, double ) ;
MAKE_ONE_TYPE( TextRise, double ) ;

MAKE_TWO_TYPE( TextFont, 		Name,	font, double, size ) ;
MAKE_TWO_TYPE( TextPosition, 	double, offx, double, offy ) ;

namespace TextMatrix
{
	enum { a, b, c, d, e, f } ;
	typedef boost::tuple<double, double, double, double, double, double> _ ;
}

MAKE_ONE_TYPE( TextString, std::string ) ;

namespace TextPosString
{
	typedef boost::variant<std::string, int> Entry ;
	typedef boost::tuple<std::vector<Entry>	>	_ ;
}

MAKE_TYPE( BeginText ) ;
MAKE_TYPE( EndText ) ;

} // end of namespace

#endif
