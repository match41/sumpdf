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

#include "Resources.hh"

#include "core/Name.hh"

#include <map>

namespace pdf {

class BaseFont ;

///	brief description
/**	The MockResources class represents
*/
class MockResources : public Resources
{
public :
	MockResources( ) ;

	Name AddFont( BaseFont *font ) ;
	BaseFont* FindFont( const Name& name ) const ;
	Name FindFont( BaseFont *font ) const ;

private :
	std::map<Name, BaseFont*>	m_font_map ;
} ;

} // end of namespace

#endif // MOCKRESOURCES_HH_
