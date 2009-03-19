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
	\file	TrueTypeFile.hh
	\brief	definition the TrueTypeFile class
	\date	Fri Mar 6 2009
	\author	Nestal Wan
*/

#ifndef __FONT_TRUE_TYPE_FILE_HEADER_INCLUDED__
#define __FONT_TRUE_TYPE_FILE_HEADER_INCLUDED__

#include <boost/cstdint.hpp>

#include <fstream>
#include <string>
#include <map>

#include "Types.hh"

namespace font {

class TrueTypeTable ;

/*!	\brief	brief description
	
	this class represents
*/
class TrueTypeFile
{
public :
	TrueTypeFile( const std::string& filename ) ;
	~TrueTypeFile( ) ;
	
	void AddTable( TrueTypeFile *src, uint32_t tag ) ;
	
	
	
private :
	void ReadTableDir( ) ;
	
	// read any integer types
	template <typename T> void Read( T& ) ;

	// skip a number of bytes
	void Skip( std::size_t count ) ;

private :
	std::ifstream	m_file ;
	std::map<std::string, TrueTypeTable>	m_tables ;
} ;

} // end of namespace

#endif
