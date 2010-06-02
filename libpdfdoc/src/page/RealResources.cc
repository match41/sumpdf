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
#include "font/BaseFont.hh"
#include "graphics/RealImage.hh"
#include "stream/Stream.hh"
#include "util/Util.hh"
#include "util/Debug.hh"

#include <boost/bind.hpp>

#include <algorithm>
#include <sstream>
#include <iostream>

namespace pdf {

RealResources::RealResources( const RealResources *parent )
	: m_parent( parent )
	, m_font_db( parent == 0 ? 0 : parent->m_font_db )
	, m_fonts( "F" )
	, m_xobjs( "X" )
	, m_states( "S" )
	, m_proc_set( 1, Name( "PDF" ) )
{
	PDF_ASSERT( parent != 0 ) ;
	PDF_ASSERT( parent->UseCount() > 0 ) ;
	PDF_ASSERT( m_font_db != 0 ) ;

    m_proc_set.push_back( Name( "Text" ) ) ;
}

RealResources::RealResources( FontDb *font_db )
	: m_parent( 0 )
	, m_font_db( font_db )
	, m_fonts( "F" )
	, m_xobjs( "X" )
	, m_states( "S" )
	, m_proc_set( 1, Name( "PDF" ) )
{
	PDF_ASSERT( m_font_db != 0 ) ;
	
    m_proc_set.push_back( Name( "Text" ) ) ;
}

RealResources* RealResources::Create(
	const RealResources		*parent,
	DictReader& 			self )
{
	RealResources *res = new RealResources( parent ) ;
	res->Read( self ) ;
	return res ;
}

void RealResources::Read( DictReader& self )
{
	ReadStateDict( self ) ;
	
	Array proc_set ;
	if ( self.Detach( "ProcSet",		proc_set ) )
		m_proc_set.assign( proc_set.begin( ), proc_set.end( ) ) ;

	ReadFontDict( self ) ;
	ReadXObject( self ) ;
}

void RealResources::ReadXObject( DictReader& self )
{
	PDF_ASSERT( self.GetFile() != 0 ) ;
	PDF_ASSERT( self.GetFile()->Pool() != 0 ) ;

	DictReader xobjs ;
	if ( self.Detach( "XObject",	xobjs ) )
	{
		m_xobjs.Clear( ) ;
		m_xobjs.MassProduce( xobjs,
			boost::bind( &CreateXObject, _1, self.GetFile() ) ) ;
	}
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
		m_states.Clear( ) ;
		m_states.MassProduce( gs, NewPtr<ExtGState>() ) ;
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
		m_fonts.Clear( ) ;
		m_fonts.MassProduce( dict, bind( &CreateFont, _1, m_font_db ) ) ;
	}
}

Ref RealResources::WriteFontDict( File *file, const FontSubsetInfo *ss ) const
{
	PDF_ASSERT( file != 0 ) ;
	PDF_ASSERT( file->Pool() != 0 ) ;

	ElementPool *pool = file->Pool( ) ;
	Dictionary font_dict ;

	for ( ResourceSet<BaseFont>::iterator i = m_fonts.begin() ;
		i != m_fonts.end() ; ++i)
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
	return m_fonts.Add( font ) ;
}

Name RealResources::AddXObject( XObject *xo )
{
	return m_xobjs.Add( xo ) ;
}

BaseFont* RealResources::FindFont( const Name& name ) const
{
	PDF_ASSERT( UseCount() > 0 ) ;

	BaseFont *f = const_cast<BaseFont*>( m_fonts.Find( name ) ) ;
	return f != 0 ? f : ( m_parent != 0 ? m_parent->FindFont( name ) : 0 ) ;
}

Name RealResources::FindFont( const BaseFont *font ) const
{
	Name n = m_fonts.Find( font ) ;
	return n != Name() ? n :
		( m_parent != 0 ? m_parent->FindFont( font ) : Name() ) ;
}

XObject* RealResources::FindXObject( const Name& name ) const
{
	PDF_ASSERT( UseCount() > 0 ) ;

	XObject *f = const_cast<XObject*>( m_xobjs.Find( name ) ) ;
	return f != 0 ? f : ( m_parent != 0 ? m_parent->FindXObject( name ) : 0 ) ;
}

Name RealResources::FindXObject( const XObject *xobj ) const
{
	Name n = m_xobjs.Find( xobj ) ;
	return n != Name() ? n :
		( m_parent != 0 ? m_parent->FindXObject( xobj ) : Name() ) ;
}

/// Throw everything away and start over.
void RealResources::Clear( )
{
	m_fonts.Clear( ) ;
	m_states.Clear( ) ;
	m_xobjs.Clear( ) ;
}

FontDb* RealResources::FontDatabase( )
{
	return m_font_db ;
}

} // end of namespace
