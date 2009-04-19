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
	\file	ElementFactory.hh
	\brief	definition the default CreateNewElement() function
	\date	Sun Apr 5 2009
	\author	Nestal Wan
*/

#ifndef __PDF_ELEMENT_FACTORY_HEADER_INCLUDED__
#define __PDF_ELEMENT_FACTORY_HEADER_INCLUDED__

namespace pdf {

class ElementReader ;

/*!	factory function for elements. This function is used to create elements.
	Sometimes it is required to create different element base on their types,
	because the actual type of the object is not known yet. This function
	provides a chance for individual element hierachy to determine which
	concrete class to be created. e.g.
	
\code
template <> BaseFont* CreateNewElement( const Object& obj, ElemenReader *src )
{
	if ( obj["Type"] == "TrueType" )
		return new TrueTypeFont( obj, src ) ;
	else if ( obj["Type"] == "Type1" )
		return new Type1Font( obj, src ) ;
	else
		throw Exception( "unknown font!" ) ;
}
\endcode
*/
template <class BaseElement>
BaseElement* CreateNewElement( const Object&, ElementReader * )
{
	return new BaseElement ;
}

} // end of namespace

#endif
