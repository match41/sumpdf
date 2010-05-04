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

/**	\file	BaseEncoding.cc
	\brief	implementation of the BaseEncoding class
	\date	Apr 24, 2010
	\author	Nestal Wan
*/

#include "BaseEncoding.hh"

#include "CodeMap.hh"

#include "core/Array.hh"
#include "core/Dictionary.hh"
#include "core/Ref.hh"
#include "file/File.hh"

#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <streambuf>

namespace pdf {

/**	constructor
	
*/
BaseEncoding::~BaseEncoding( )
{
}

wchar_t BaseEncoding::ToUnicode( unsigned short char_code ) const
{
	// pass-through if the character code is not found
	CharMap::left_const_iterator i = m_charmap.left.find( char_code ) ;
	return i != m_charmap.left.end() ? i->second :
		static_cast<wchar_t>( char_code ) ; 
}

unsigned short BaseEncoding::FromUnicode( wchar_t unicode ) const
{
	CharMap::right_const_iterator i = m_charmap.right.find( unicode ) ;
	return i != m_charmap.right.end() ? i->second :
		static_cast<unsigned short>( unicode ) ; 
}

void BaseEncoding::Add( unsigned short char_code, wchar_t unicode )
{
	m_charmap.insert( CharMap::value_type(
		static_cast<unsigned short>( char_code ), unicode ) ) ;
}

/// avoid stupid sign extension when converting char to unsigned short
unsigned short BaseEncoding::Ext( char ch )
{
	return static_cast<unsigned char>( ch ) ;
}

std::wstring BaseEncoding::Decode( const std::string& bytes ) const
{
	std::wstring result( bytes.size(), ' ' ) ;
	std::transform( bytes.begin(), bytes.end(), result.begin(),
		boost::bind( &BaseEncoding::ToUnicode,
			this,
			boost::bind( &BaseEncoding::Ext, _1 ) ) ) ;
	return result ;
}

std::size_t BaseEncoding::Encode(
	std::wstring::const_iterator first,
	std::wstring::const_iterator last,
	std::ostream& out ) const
{
	std::size_t count = 0 ;
	for ( std::wstring::const_iterator i = first ; i != last ; ++i )
	{
		out.rdbuf()->sputc( static_cast<char>(FromUnicode(*i) ) ) ;
		count++ ;
	}
	return count ;
}

BaseEncoding::iterator BaseEncoding::begin( )
{
	return m_charmap.left.begin( ) ;
}

BaseEncoding::iterator BaseEncoding::end( )
{
	return m_charmap.left.end( ) ;
}

BaseEncoding::const_iterator BaseEncoding::begin( ) const
{
	return m_charmap.left.begin( ) ;
}

BaseEncoding::const_iterator BaseEncoding::end( ) const
{
	return m_charmap.left.end( ) ;
}

Object BaseEncoding::Write( File *file ) const
{
	Dictionary self ;
//	self.insert( "BaseEncoding", 	m_base ) ;
	self.insert( "Type",			"Encoding" ) ;
	wchar_t current = 0 ;
	
	Array diff ;
	// write all glyph name pairs. it is a big waste of space.
	// TODO: save space later
	for ( const_iterator i = begin() ; i != end() ; ++i )
	{
		const char *char_name = 0 ;
		bool r = UnicodeToName( i->second, char_name ) ;
		PDF_ASSERT( r ) ;
		PDF_ASSERT( char_name != 0 ) ;

		if ( current == 0 || i->first != current + 1 )
			diff.push_back( i->first ) ;
		
		diff.push_back( Name(char_name) ) ;
		current = i->first ;
	}
	if ( !diff.empty() )
		self.insert( "Differences", diff ) ;
	
	return file->WriteObj( self ) ;
}

} // end of namespace
