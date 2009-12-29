/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	PaintOp.hh
	\brief	definition the PaintOp class
	\date	Mon Mar 2 2009
	\author	Nestal Wan
*/

#ifndef __PDF_PAINT_OP_HEADER_INCLUDED__
#define __PDF_PAINT_OP_HEADER_INCLUDED__

#include "TextOps.hh"
#include "PathSegment.hh"

#include "util/Exception.hh"

#include <boost/variant.hpp>

#include <map>
#include <iosfwd>

namespace pdf {

class Object ;
class Resources ;

/*!	\brief	paint operations
	
	This class represents the paint operations used to paint a page in an PDF
	document. These operations are stored in the page's content streams.
*/
class PaintOp
{
public :
	class DecodeError : public Exception
	{
	public :
		DecodeError( const char *type ) ;
	} ;

	PaintOp(
		const std::string&	ops,
		Object 				*args,
		std::size_t 		count,
		Resources 			*r ) ;

    friend std::ostream& operator<<( std::ostream& os, const PaintOp& op ) ;

private :
    // function pointer mapping
    typedef void (PaintOp::*FuncPtr)( Object*, std::size_t, Resources* ) ;
	static const std::pair<const std::string, FuncPtr> m_table[] ;

	typedef std::map<std::string, FuncPtr> FuncMap ;
	static const FuncMap m_func_map ;

private :
	template <typename Op>
	void Decode( Object *args, std::size_t count, Resources *r ) ;

private :
	boost::variant<
		CharSpace,
		WordSpace, 
		TextScale, 
		TextLeading,
		TextRender,
		TextRise, 
		TextFont,
		TextPosition,
		TextMatrix,
		TextString,
		TextPosString,
		BeginText,
		EndText>
			m_ops ;
} ;

} // end of namespace

#endif
