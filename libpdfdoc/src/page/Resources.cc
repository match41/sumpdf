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
	\file	Resources.cc
	\brief	implementation the Resources class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#include "Resources.hh"


#include "XObject.hh"

#include "core/Array.hh"
#include "file/IFile.hh"
#include "file/ObjectReader.hh"
#include "file/RefObjMap.hh"
#include "file/ResourcePool.hh"
#include "font/BaseFont.hh"
#include "util/Util.hh"
#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <sstream>

namespace pdf {

Resources::Resources( const Resources *parent )
	: m_parent( parent ),
	  m_ft_lib( parent == 0 ? 0 : parent->m_ft_lib ),
	  m_proc_set( 1, Name( "PDF" ) )
{
	PDF_ASSERT( parent != 0 ) ;
	PDF_ASSERT( m_ft_lib != 0 ) ;
	
    m_proc_set.push_back( Name( "Text" ) ) ;
}

Resources::Resources( FT_Library ft_lib )
	: m_parent( 0 ),
	  m_ft_lib( ft_lib ),
	  m_proc_set( 1, Name( "PDF" ) )
{
	PDF_ASSERT( m_ft_lib != 0 ) ;
	
    m_proc_set.push_back( Name( "Text" ) ) ;
}

Resources::~Resources( )
{
	using namespace boost ;
	std::for_each( m_fonts.left.begin(), m_fonts.left.end(),
		bind( &BaseFont::Release,
			bind( &FontMap::left_value_type::second, _1 ) ) ) ;
}

void Resources::Read( const Dictionary& dict, IFile *file )
{
	Dictionary self = dict ;

	Dictionary ext_gstate ;
	Array proc_set ;
	Detach( file, self, "ExtGState",	ext_gstate ) ;
	Detach( file, self, "ProcSet",		proc_set ) ;
	m_proc_set.assign( proc_set.begin( ), proc_set.end( ) ) ;

	ReadFontDict( self, file ) ;
//	ReadSubDict( "XObject", file, m_xobjs ) ;

	m_self.Read( self, file ) ;
	m_ext_gstate.Read( ext_gstate, file ) ;
}

Ref Resources::Write( IFile *file ) const
{
	CompleteObj copy( m_self ) ;
	Dictionary& dict = copy.Get() ;
    
	dict["ProcSet"]	= Array( m_proc_set.begin( ), m_proc_set.end( ) ) ;
	dict["Font"]	= WriteFontDict( file ) ;
//	dict["XObject"]	= WriteSubDict( m_xobjs, repo ) ;

    return copy.Write( file ) ;
}

void Resources::ReadFontDict( Dictionary& self, IFile *file )
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( file->Pool() != 0 ) ;
	PDF_ASSERT( m_ft_lib != 0 ) ;

	Dictionary dict ;
	if ( Detach( file, self, "Font", dict ) )
	{
		FontPool *font_pool = &file->Pool( )->fonts ;
		for ( Dictionary::iterator i  = dict.begin( ) ; i != dict.end( ) ; ++i )
		{
			BaseFont *font = 0 ;
			if ( i->second.Is<Ref>() )
			{
				const Ref& link = i->second.As<Ref>() ;
				font = font_pool->Find( link ) ;
				if ( font == 0 )
				{
					Dictionary self = file->ReadObj( link ) ;
					font = CreateFont( self, file, m_ft_lib ) ; 
					font_pool->Add( link, font ) ;
				}
				PDF_ASSERT( font != 0 ) ;
			}
			
			// the font is not an indirect object, so it can't be shared.
			else if ( i->second.Is<Dictionary>() )
				font = CreateFont( i->second.As<Dictionary>(), file, m_ft_lib ) ;

			m_fonts.insert( FontMap::value_type(i->first, font) ) ;
		}
	}
}

Ref Resources::WriteFontDict( IFile *file ) const
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( file->Pool() != 0 ) ;

	FontPool *pool = &file->Pool( )->fonts ;
	Dictionary font_dict ;
	
	for ( FontMap::left_const_iterator i = m_fonts.left.begin(); i != m_fonts.left.end() ; ++i)
	{
		PDF_ASSERT( i->second != 0 ) ;
	
		Ref link = pool->Find( i->second ) ;
		if ( link == Ref() )
			link = i->second->Write( file ) ;
		
		font_dict.insert( std::make_pair( i->first, link ) ) ;
	}
	
	return file->WriteObj( font_dict ) ;
}

XObject* Resources::ReadXObj( const Ref& link )
{
	// XObjects are made from streams, so we read them as streams
	return 0 ;
}

Name Resources::AddFont( BaseFont *font )
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

BaseFont* Resources::FindFont( const Name& name ) const
{
	FontMap::left_const_iterator i = m_fonts.left.find( name ) ;
	return i != m_fonts.left.end() ? i->second : 0 ;
}

Name Resources::FindFont( BaseFont *font ) const
{
	FontMap::right_const_iterator i = m_fonts.right.find( font ) ;
	return i != m_fonts.right.end() ? i->second : Name() ;
}

} // end of namespace
