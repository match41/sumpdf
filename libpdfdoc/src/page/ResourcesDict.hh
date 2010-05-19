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

/**	\file	ResourcesDict.hh
    \brief	definition the Resources class
    \date	Jan 23, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_RESOURCES_HH_EADER_INCLUDED__
#define __PDF_RESOURCES_HH_EADER_INCLUDED__

namespace pdf {

class Name ;
class BaseFont ;
class XObject ;

///	brief description
/**	The Resources class represents
*/
class ResourcesDict
{
protected :
	~ResourcesDict( ) ;

public :
	virtual Name AddFont( BaseFont *font ) = 0 ;
	virtual BaseFont* FindFont( const Name& name ) const = 0 ;
	virtual Name FindFont( const BaseFont *font ) const = 0 ;
	
	virtual Name AddXObject( XObject *xobj ) = 0 ;
	virtual Name FindXObject( const XObject *xobj ) const = 0 ;
	virtual XObject* FindXObject( const Name& name ) const = 0 ;
} ;

} // end of namespace

#endif // RESOURCES_HH_
