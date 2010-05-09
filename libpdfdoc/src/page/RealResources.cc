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
	\file	RealResources.cc
	\brief	implementation the Resources class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#include "RealResources.hh"

#include "ExtGState.hh"

#include "core/Array.hh"
#include "file/File.hh"
#include "file/DictReader.hh"
#include "file/ElementPool.hh"
#include "file/ElementFactory.hh"
#include "font/BaseFont.hh"
#include "util/Util.hh"
#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <sstream>
#include <iostream>

namespace pdf {

RealResources::RealResources( const RealResources *parent )
	: m_parent( parent ),
	  m_font_db( parent == 0 ? 0 : parent->m_font_db ),
	  m_proc_set( 1, Name( "PDF" ) )
{
	PDF_ASSERT( parent != 0 ) ;
	PDF_ASSERT( parent->UseCount() > 0 ) ;
	PDF_ASSERT( m_font_db != 0 ) ;

    m_proc_set.push_back( Name( "Text" ) ) ;
}

RealResources::RealResources( FontDb *font_db )
	: m_parent( 0 ),
	  m_font_db( font_db ),
	  m_proc_set( 1, Name( "PDF" ) )
{
	PDF_ASSERT( m_font_db != 0 ) ;
	
    m_proc_set.push_back( Name( "Text" ) ) ;
}

RealResources::~RealResources( )
{
	using namespace boost ;
	std::for_each( m_fonts.left.begin(), m_fonts.left.end(),
		bind( &BaseFont::Release,
			bind( &FontMap::left_value_type::second, _1 ) ) ) ;

	using namespace boost ;
	std::for_each( m_states.left.begin(), m_states.left.end(),
		bind( &ExtGState::Release,
			bind( &StateMap::left_value_type::second, _1 ) ) ) ;
}

void RealResources::Read( DictReader& self )
{
	ReadStateDict( self ) ;
	
	Array proc_set ;
	if ( self.Detach( "ProcSet",		proc_set ) )
		m_proc_set.assign( proc_set.begin( ), proc_set.end( ) ) ;

	ReadFontDict( self ) ;
}

void RealResources::ReadXObject( DictReader& self )
{
}

Ref RealResources::Write( File *file, const FontSubsetInfo *subset ) const
{
	Dictionary dict ;
    
	dict.insert( "ProcSet",	m_proc_set ) ;
	dict.insert( "Font",	WriteFontDict( file, subset ) ) ;

    return file->WriteObj( dict ) ;
}

void RealResources::ReadStateDict( DictReader& self )
{
	PDF_ASSERT( self.GetFile() != 0 ) ;
	PDF_ASSERT( self.GetFile()->Pool() != 0 ) ;
	
	DictReader gs ;
	if ( self.Detach( "ExtGState",	gs ) )
	{
		// clear the states before
		using namespace boost ;
		std::for_each( m_states.left.begin(), m_states.left.end(),
			bind( &ExtGState::Release,
				bind( &StateMap::left_value_type::second, _1 ) ) ) ;
		m_states.clear( ) ;

		ElementFactory<> factory( gs ) ;
		factory.MassProduce<ExtGState>(
			NewPtr<ExtGState>(),
			std::inserter( m_states.left, m_states.left.end() ) ) ;
	}
}

void RealResources::ReadFontDict( DictReader& self )
{
	PDF_ASSERT( self.GetFile() != 0 ) ;
	PDF_ASSERT( self.GetFile()->Pool() != 0 ) ;
	PDF_ASSERT( m_font_db != 0 ) ;

	DictReader dict ;
	if ( self.Detach( "Font", dict ) )
	{
		using namespace boost ;
		std::for_each( m_fonts.left.begin(), m_fonts.left.end(),
			bind( &BaseFont::Release,
				bind( &FontMap::left_value_type::second, _1 ) ) ) ;
		m_fonts.clear( ) ;

		ElementFactory<> factory( dict ) ;
		factory.MassProduce<BaseFont>(
			bind( &CreateFont, _1, m_font_db ),
			std::inserter( m_fonts.left, m_fonts.left.end() ) ) ;
	}
}

Ref RealResources::WriteFontDict( File *file, const FontSubsetInfo *ss ) const
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( file->Pool() != 0 ) ;

	ElementPool *pool = file->Pool( ) ;
	Dictionary font_dict ;

	for ( FontMap::left_const_iterator i = m_fonts.left.begin() ;
		i != m_fonts.left.end() ; ++i)
	{
		PDF_ASSERT( i->second != 0 ) ;
	
		Ref link = pool->Find( i->second ) ;
		if ( link == Ref() )
		{
			link = i->second->Write( file, ss ) ;
			pool->Add( link, i->second ) ;
		}
		
		font_dict.insert( std::make_pair( i->first, link ) ) ;
	}
	
	return file->WriteObj( font_dict ) ;
}

Name RealResources::AddFont( BaseFont *font )
{
	// first, see if the font is already added
	using namespace boost ;
	FontMap::right_iterator it = m_fonts.right.find( font ) ;
	if ( it != m_fonts.right.end( ) )
		return it->second ;

	std::size_t idx = m_fonts.size( ) ;

	// create a new name
	Name name ;
	do
	{
		std::ostringstream oss ;
		oss << "F" << idx++ ;
		name = Name( oss.str() ) ;

	} while ( m_fonts.left.find( name ) != m_fonts.left.end( ) ) ;

	m_fonts.insert( FontMap::value_type( name, font ) ) ;
	font->AddRef() ;
	
	return name ;
}

BaseFont* RealResources::FindFont( const Name& name ) const
{
	PDF_ASSERT( UseCount() > 0 ) ;

	FontMap::left_const_iterator i = m_fonts.left.find( name ) ;
	return i != m_fonts.left.end() ? i->second :
		( m_parent != 0 ? m_parent->FindFont( name ) : 0 ) ;
}

Name RealResources::FindFont( const BaseFont *font ) const
{
	// we don't modify it anyway. it is for searching in the map
	BaseFont *ncfont = const_cast<BaseFont*>( font ) ;

	FontMap::right_const_iterator i = m_fonts.right.find( ncfont ) ;
	return i != m_fonts.right.end() ? i->second :
		( m_parent != 0 ? m_parent->FindFont( font ) : Name() ) ; 
}

/// Throw everything away and start over.
void RealResources::Clear( )
{
	m_fonts.clear( ) ;
	m_states.clear( ) ;
}

} // end of namespace
