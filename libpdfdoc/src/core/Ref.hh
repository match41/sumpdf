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
	\file	Ref.hh
	\brief	definition the Ref class
	\date	Wed Mar 5 2008
	\author	Nestal Wan
*/

#ifndef __PDF_INDIRECT_OBJ_HEADER_INCLUDED__
#define __PDF_INDIRECT_OBJ_HEADER_INCLUDED__

#include <iosfwd>

namespace pdf {

class Token ;
class TokenSrc ;

/*!	\brief	Indirect objects in PDF
	
	This class represents indirect objects according to PDF specification.
*/
class Ref
{
public :
	Ref( ) ;
	Ref( std::size_t id, std::size_t gen ) ;

	std::size_t ID( ) const ;
	std::size_t Gen( ) const ;
	
	bool operator==( const Ref& obj ) const ;
	bool operator!=( const Ref& obj ) const ;
	
	friend std::istream& operator>>( std::istream& is, Ref& obj ) ;
	friend std::ostream& operator<<( std::ostream& is, const Ref& obj );
	friend TokenSrc& operator>>( TokenSrc& src, Ref& obj ) ;
	
private :
	// These two types should be std::size_t, but I want to save space
	// in 64-bit systems. If both members are 64 bits long, Ref will be
	// the biggest in the Object variant, making all object bigger.
	unsigned m_obj_id ;
	unsigned m_generation ;
} ;

bool operator<( const Ref& obj1, const Ref& obj2 ) ;
bool operator>( const Ref& obj1, const Ref& obj2 ) ;

} // end of namespace

#endif
