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
	\file	PaintOp.cc
	\brief	implementation the PaintOp class
	\date	Mon Mar 2 2009
	\author	Nestal Wan
*/

#include "PaintOp.hh"

namespace pdf {

PaintOp::PaintOp( const std::string& ops, const Object *args,
                                          std::size_t count )
{
/*	typedef bool (Object::*FuncPtr)( std::istream&, std::streampos ) ;
	
	static const std::pair<const Token, FuncPtr> table[] =
	{
		std::make_pair( "BT",		&Object::DecodeObject<Dictionary> ),
		std::make_pair( Token( "[" ),		&Object::DecodeObject<Array> ),
		std::make_pair( Token( "(" ),		&Object::DecodeObject<String> ),
		std::make_pair( Token( "<" ),		&Object::DecodeObject<String> ),
		std::make_pair( Token( "/" ),		&Object::DecodeObject<Name> ),
		std::make_pair( Token( "true" ),	&Object::DecodeObject<Bool> ),
		std::make_pair( Token( "false" ),	&Object::DecodeObject<Bool> ),
	} ;
	typedef std::map<Token, FuncPtr> FuncMap ;
	static const FuncMap map( Begin( table ), End( table ) ) ;
*/
}

} // end of namespace
