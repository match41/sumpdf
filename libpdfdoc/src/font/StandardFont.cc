/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
	\file	StandardFont.cc
	\brief	implementation the StandardFont class
	\date	Sun Mar 8 2009
	\author	Nestal Wan
*/

#include "StandardFont.hh"

#include "core/Dictionary.hh"
#include "file/IFile.hh"
#include "util/Util.hh"

#include <set>

namespace pdf {

StandardFont::StandardFont( const Name& name )
	: m_font_name( name )
{
}

std::string StandardFont::BaseName( ) const
{
	return m_font_name.Str() ;
}

bool StandardFont::IsStandardFont( const Name& font_name )
{
	static const Name std_font_name[] =
	{
		"Courier",				"CourierNew",
		"Courier-Oblique",		"CourierNew,Italic",
		"Courier-Bold",			"CourierNew,Bold",
		"Courier-BoldOblique",	"CourierNew,BoldItalic",
		"Helvetica",			"Arial",
		"Helvetica-Oblique",	"Arial,Italic",
		"Helvetica-Bold",		"Arial,Bold",
		"Helvetica-BoldOblique","Arial,BoldItalic",
		"Times-Roman",			"TimesNewRoman",
		"Times-Italic",			"TimesNewRoman,Italic",
		"Times-Bold",			"TimesNewRoman,Bold",
		"Times-BoldItalic",		"TimesNewRoman,BoldItalic",
		"Symbol",
		"ZapfDingbats",
	} ;

	static const std::set<Name> s( Begin(std_font_name), End(std_font_name) ) ;
	return s.find( font_name ) != s.end( ) ;
}

Ref StandardFont::Write( IFile *file ) const
{
	// the standard 14 font dictionary is much simpler
	Dictionary dict ;
	dict["Type"]		= Name( "Font" ) ;
	dict["Subtype"]		= Name( "Type1" ) ;
	dict["BaseFont"]	= m_font_name ;
	return file->WriteObj( dict ) ;
}

// freetype library is optional
#ifndef HAVE_FREETYPE
template <> BaseFont* CreateNewElement( const Object& , IElementSrc * )
{
	return new StandardFont( "Arial" ) ;
}
#endif

} // end of namespace
