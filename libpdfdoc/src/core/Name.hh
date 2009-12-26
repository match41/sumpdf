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

/*!
	\file	Name.hh
	\brief	definition the Name class
	\date	Thu Mar 13 2008
	\author	Nestal Wan
*/

#ifndef __PDF_NAME_HEADER_INCLUDED__
#define __PDF_NAME_HEADER_INCLUDED__

#include <string>

namespace pdf {

class Object ;
class Token ;
class TokenSrc ;

/*!	\brief	PDF name object
	\internal
	
	This class represents a PDF name object. A PDF name object is a string
	starts with the '/' character. It is used as identifiers in the PDF files,
	e.g. the keys to PDF dictionaries.
*/
class Name
{
private :
	std::string	m_name ;

public :
	Name( const std::string& name = std::string() ) ;
	Name( const char *name ) ;
	Name( const Object& obj ) ;

	void swap( Name& name ) ;

	friend std::istream& operator>>( std::istream& is, Name& name ) ;
	friend std::ostream& operator<<( std::ostream& os, const Name& name ) ;
	friend TokenSrc& operator>>( TokenSrc& src, Name& dict ) ;
	
	friend bool operator<( const Name& name1, const Name& name2 ) ;
	
	friend bool operator==( const Name& name1, const Name& name2 ) ;
	friend bool operator!=( const Name& name1, const Name& name2 ) ;
	
	const std::string& Str() const ;
	
	bool empty( ) const ;
} ;

} // end of namespace

namespace std
{
	void swap( pdf::Name& dict1, pdf::Name& dict2 ) ;
}

#endif
