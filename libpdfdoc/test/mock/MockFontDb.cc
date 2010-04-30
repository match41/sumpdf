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

/**	\file	MockFontDb.cc
	\brief	implementation of the MockFontDb class
	\date	Feb 12, 2010
	\author	Nestal Wan
*/

#include "MockFontDb.hh"

#include <fstream>
#include <iterator>

namespace pdfut {

using namespace pdf ;

/**	constructor
	
*/
MockFontDb::MockFontDb( )
	: m_weight( font::normal_weight ),
	  m_slant( font::roman ),
	  m_width( font::normal_width )
{
}

std::vector<unsigned char> MockFontDb::FindFont(
	const std::string&	base_name,
	font::Weight		weight,
	font::Slant			slant,
	font::Width			width )
{
	// save the query
	m_basename	= base_name ;
	m_weight	= weight ;
	m_width		= width ;
	m_slant		= slant ;
	
	std::string file = std::string(TEST_DATA_DIR) +"FreeMonoBoldOblique.ttf" ;
	
	std::ifstream fs( file.c_str(), std::ios::binary | std::ios::in ) ;
	return std::vector<unsigned char>(
		(std::istreambuf_iterator<char>( fs )),
		(std::istreambuf_iterator<char>()) ) ;
}

std::vector<unsigned char> MockFontDb::FindFont( FT_FaceRec_ *face )
{
	std::string file = std::string(TEST_DATA_DIR) +"FreeMonoBoldOblique.ttf" ;
	
	std::ifstream fs( file.c_str(), std::ios::binary | std::ios::in ) ;
	return std::vector<unsigned char>(
		(std::istreambuf_iterator<char>( fs )),
		(std::istreambuf_iterator<char>()) ) ;
}

std::string MockFontDb::LastQueryName( ) const
{
	return m_basename ;
}

font::Weight MockFontDb::LastQueryWeight( ) const
{
	return m_weight ;
}

font::Slant MockFontDb::LastQuerySlant( ) const
{
	return m_slant ;
}

font::Width MockFontDb::LastQueryWidth( ) const
{
	return m_width ;
}

} // end of namespace
