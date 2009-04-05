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
	\file	PaintOp.hh
	\brief	definition the PaintOp class
	\date	Mon Mar 2 2009
	\author	Nestal Wan
*/

#ifndef __PDF_PAINT_OP_HEADER_INCLUDED__
#define __PDF_PAINT_OP_HEADER_INCLUDED__

#include "TextOps.hh"
#include "PathSegment.hh"

namespace pdf {

class Object ;

/*!	\brief	brief description
	
	this class represents
*/
class PaintOp
{
public :
	PaintOp( const std::string& ops, const Object *args, std::size_t count ) ;

private :
	template <typename Op>
	void DecodeNoArg( const Object *args, std::size_t count ) ;

private :
	boost::variant<TextState, TextPosition, TextMatrix, TextString,
	               TextPosString, BeginText, EndText> m_ops ;
} ;

} // end of namespace

#endif
