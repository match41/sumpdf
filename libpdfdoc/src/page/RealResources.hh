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

#include "Resources.hh"
#include "util/RefCounter.hh"

#include "core/Dictionary.hh"
#include "core/Name.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

#include <vector>

namespace pdf {

class File ;
class BaseFont ;
class DictReader ;
class RealImage ;
class Object ;
class Ref ;

/*!	\brief	page resources

	Resources are PDF objects used for rendering but not saved in the content
	streams. These objects are referred by names in the content stream. The
	resources dictionary maps their names to the actual objects.
*/
class RealResources : public Resources, public RefCounter
{
public :
	explicit RealResources( const RealResources *parent ) ;
	explicit RealResources( FT_Library ft_lib ) ;
	~RealResources( ) ;

	Name AddFont( BaseFont *font ) ;

	void Read( DictReader& self ) ;
	Ref  Write( File *file ) const ;

	BaseFont* FindFont( const Name& name ) const ;
	Name FindFont( const BaseFont *font ) const ;

private :
	void ReadFontDict( DictReader& self ) ;
	Ref WriteFontDict( File *file ) const ;

private :
	const RealResources	*m_parent ;
	FT_Library		m_ft_lib ;

	typedef	boost::bimap<
		boost::bimaps::set_of<Name>,
		boost::bimaps::set_of<BaseFont*>
	> FontMap ; 
	FontMap				m_fonts ;

	typedef std::map<Name, RealImage*> ImageMap ;
	ImageMap			m_images ;

	std::vector<Name>	m_proc_set ;
} ;

} // end of namespace

#endif
