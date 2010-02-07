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

#include "core/Array.hh"
#include "file/File.hh"
#include "file/DictReader.hh"
#include "file/RefObjMap.hh"
#include "file/ElementPool.hh"
#include "font/BaseFont.hh"
#include "font/FreeTypeWrappers.hh"
#include "util/Util.hh"
#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <sstream>
#include <iostream>

namespace pdf {

RealResources::RealResources( const RealResources *parent )
	: m_parent( parent ),
	  m_ft_lib( parent == 0 ? 0 : parent->m_ft_lib ),
	  m_proc_set( 1, Name( "PDF" ) )
{
	PDF_ASSERT( parent != 0 ) ;
	PDF_ASSERT( m_ft_lib != 0 ) ;
	
    m_proc_set.push_back( Name( "Text" ) ) ;
}

RealResources::RealResources( FT_Library ft_lib )
	: m_parent( 0 ),
	  m_ft_lib( ft_lib ),
	  m_proc_set( 1, Name( "PDF" ) )
{
	PDF_ASSERT( m_ft_lib != 0 ) ;
	
    m_proc_set.push_back( Name( "Text" ) ) ;
}

RealResources::~RealResources( )
{
	using namespace boost ;
	std::for_each( m_fonts.left.begin(), m_fonts.left.end(),
		bind( &BaseFont::Release,
			bind( &FontMap::left_value_type::second, _1 ) ) ) ;
}

void RealResources::Read( DictReader& self )
{
	Dictionary ext_gstate ;
	Array proc_set ;
	self.Detach( "ExtGState",	ext_gstate ) ;
	self.Detach( "ProcSet",		proc_set ) ;
	m_proc_set.assign( proc_set.begin( ), proc_set.end( ) ) ;

	ReadFontDict( self ) ;
}

Ref RealResources::Write( File *file ) const
{
	Dictionary dict ;
    
	dict["ProcSet"]	= Array( m_proc_set.begin( ), m_proc_set.end( ) ) ;
	dict["Font"]	= WriteFontDict( file ) ;

    return file->WriteObj( dict ) ;
}

void RealResources::ReadFontDict( DictReader& self )
{
	PDF_ASSERT( self.GetFile() != 0 ) ;
	PDF_ASSERT( self.GetFile()->Pool() != 0 ) ;
	PDF_ASSERT( m_ft_lib != 0 ) ;

	DictReader dict ;
	if ( self.Detach( "Font", dict ) )
	{
		ElementPool *pool = self.GetFile()->Pool( ) ;
		for ( Dictionary::iterator i  = dict->begin( ) ; i != dict->end( ) ; ++i )
		{
			ft::Library lib = { m_ft_lib } ;
			
			BaseFont *font = 0 ;
			
			Ref link = i->second.To<Ref>( std::nothrow ) ;
			if ( !pool->Acquire( link, font ) )  
			{
				DictReader font_dict ;
				dict.At( i, font_dict ) ;
				font = CreateFont( font_dict, lib ) ;
				pool->Add( link, font ) ; 
			}

			m_fonts.insert( FontMap::value_type(i->first, font) ) ;
		}
	}
}

Ref RealResources::WriteFontDict( File *file ) const
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
			link = i->second->Write( file ) ;
		
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

	// create a new name
	Name name ;
	do
	{
		std::ostringstream oss ;
		oss << "F" << m_fonts.size( ) ;
		name = Name( oss.str() ) ;
	} while ( m_fonts.left.find( name ) != m_fonts.left.end( ) ) ;

	m_fonts.insert( FontMap::value_type( name, font ) ) ;
	return name ;
}

BaseFont* RealResources::FindFont( const Name& name ) const
{
	FontMap::left_const_iterator i = m_fonts.left.find( name ) ;
	return i != m_fonts.left.end() ? i->second :
		( m_parent != 0 ? m_parent->FindFont( name ) : 0 ) ;
}

Name RealResources::FindFont( const BaseFont *font ) const
{
	BaseFont *ncfont = const_cast<BaseFont*>( font ) ;

	FontMap::right_const_iterator i = m_fonts.right.find( ncfont ) ;
	return i != m_fonts.right.end() ? i->second :
		( m_parent != 0 ? m_parent->FindFont( font ) : Name() ) ; 
}

} // end of namespace
