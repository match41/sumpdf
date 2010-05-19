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

/**	\file	MockResources.hh
    \brief	definition the MockResources class
    \date	Jan 23, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_MOCKRESOURCES_HH_EADER_INCLUDED__
#define __PDF_MOCKRESOURCES_HH_EADER_INCLUDED__

#include "ResourcesDict.hh"

#include "core/Name.hh"

#include <map>

namespace pdf {

class BaseFont ;

///	brief description
/**	The MockResources class represents
*/
class MockResources : public ResourcesDict
{
public :
	MockResources( ) ;

	Name AddFont( BaseFont *font ) ;
	BaseFont* FindFont( const Name& name ) const ;
	Name FindFont( const BaseFont *font ) const ;

	Name AddXObject( XObject *xobj ) ;
	Name FindXObject( const XObject *xobj ) const ;
	XObject* FindXObject( const Name& name ) const ;

	void Add( const Name& name, BaseFont *font ) ;
	void Add( const Name& name, XObject *xo ) ;

private :
	std::map<Name, BaseFont*>	m_font_map ;
	std::map<Name, XObject*>	m_xobj_map ;
} ;

} // end of namespace

#endif // MOCKRESOURCES_HH_
