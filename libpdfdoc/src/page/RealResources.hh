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

#include "core/Dictionary.hh"
#include "core/Name.hh"
#include "file/CompleteObj.hh"

// freetype headers
#include <ft2build.h>
#include FT_FREETYPE_H

#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

#include <vector>

namespace pdf {

class File ;
class BaseFont ;
class RealImage ;
class XObject ;
class Object ;

/*!	\brief	page resources

	Resources are PDF objects used for rendering but not saved in the content
	streams. These objects are referred by names in the content stream. The
	resources dictionary maps their names to the actual objects.
*/
class RealResources : public Resources
{
public :
	explicit RealResources( const RealResources *parent ) ;
	explicit RealResources( FT_Library ft_lib ) ;
	~RealResources( ) ;

	Name AddFont( BaseFont *font ) ;

	void Read( const Dictionary& self_obj, File *file ) ;
	Ref  Write( File *file ) const ;

	BaseFont* FindFont( const Name& name ) const ;
	Name FindFont( BaseFont *font ) const ;

private :
	void ReadFontDict( Dictionary& self, File *file ) ;
	Ref WriteFontDict( File *file ) const ;

	XObject* ReadXObj( const Ref& link ) ;

private :
	const RealResources	*m_parent ;
	FT_Library		m_ft_lib ;

//	CompleteObj		m_self ;
//	CompleteObj		m_ext_gstate ;

	typedef	boost::bimap<
		boost::bimaps::set_of<Name>,
		boost::bimaps::set_of<BaseFont*>
	> FontMap ; 
	FontMap				m_fonts ;

	typedef std::map<Name, RealImage*> ImageMap ;
	ImageMap			m_images ;

	std::map<Name, XObject*>	m_xobjs ;

	std::vector<Name>	m_proc_set ;
} ;

} // end of namespace

#endif
