/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	Resources.hh
	\brief	definition the Resources class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#ifndef __PDF_RESOURCES_HEADER_INCLUDED__
#define __PDF_RESOURCES_HEADER_INCLUDED__

#include "ResourcesDict.hh"
#include "util/RefCounter.hh"

#include "ResourceSet.hh"

#include "core/Dictionary.hh"
#include "core/Name.hh"

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

#include <vector>

struct FT_LibraryRec_ ;

namespace pdf {

class BaseFont ;
class File ;
class Font ;
class FontDb ;
class FontSubsetInfo ;
class DictReader ;
class RealColorSpace ;
class RealImage ;
class Object ;
class Ref ;
class ExtGState ;
class XObject ;

/*!	\brief	page resources

	Resources are PDF objects used for rendering but not saved in the content
	streams. These objects are referred by names in the content stream. The
	resources dictionary maps their names to the actual objects.
*/
class RealResources : public ResourcesDict, public RefCounter
{
public :
	typedef DictReader BaseType ;

public :
	explicit RealResources( const RealResources *parent ) ;
	explicit RealResources( FontDb *fontdb ) ;

	static RealResources* Create( const RealResources *parent, DictReader& self);

	Name AddFont( BaseFont *font ) ;
	Name AddXObject( XObject *xobj ) ;
	
	void Read( DictReader& self ) ;
	Ref  Write( File *file, const FontSubsetInfo *ss ) const ;

	BaseFont* FindFont( const Name& name ) const ;
	Name FindFont( const BaseFont *font ) const ;

	XObject* FindXObject( const Name& name ) const ;
	Name FindXObject( const XObject *xobj ) const ;

	ColorSpace* FindColorSpace( const Name& name ) const ;

	void Clear( ) ;

	FontDb* FontDatabase( ) ;

private :
	void ReadFontDict( DictReader& self ) ;
	Ref WriteFontDict( File *file, const FontSubsetInfo *ss ) const ;

	void ReadStateDict( DictReader& self ) ;
	void ReadXObject( DictReader& self ) ;
	void ReadColorSpace( DictReader& self ) ;

	Ref WriteXObject( File *file ) const ;

private :
	const RealResources	*const m_parent ;
	FontDb				*m_font_db ;

	ResourceSet<BaseFont>		m_fonts ;
	ResourceSet<XObject>		m_xobjs ;
	ResourceSet<ExtGState>		m_states ;
	ResourceSet<RealColorSpace>	m_color_spaces ;

	std::vector<Name>	m_proc_set ;
} ;

} // end of namespace

#endif
