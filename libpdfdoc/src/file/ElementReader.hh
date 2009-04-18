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
	\file	ElementReader.hh
	\brief	definition the ElementReader class
	\date	Sat Apr 18 2009
	\author	Nestal Wan
*/

#ifndef __PDF_ELEMENT_READER_INCLUDED__
#define __PDF_ELEMENT_READER_INCLUDED__

#include "core/Ref.hh"
#include "core/Object.hh"

#include "ElementFactory.hh"

#include <map>

namespace pdf {

class IFile ;
class IElement ;

/*!	\brief	brief description
	
	this class represents
*/
class ElementReader
{
public :
	ElementReader( IFile *file ) ;

	IElement* Find( const Ref& link ) ;
	
	/*!	helper function to create elements. This function is the primary
		function for users of this class to read elements.
	*/
	template <class Element>
	Element* Read( const Ref& link )
	{
		IElement *temp = Find( link ) ;
		
		// element found in cache, try to re-use it
		if ( temp != 0 )
			// dynamic cast reference
			// it will throw bad_cast if failed
			return &dynamic_cast<Element&>( *temp ) ;

		else
		{
			Object obj = ReadObj( link ) ;
			Element *element = CreateNewElement<Element>( obj, this ) ;

			Store( element, obj, link ) ;
			return element ;
		}
	}

private :
	Object ReadObj( const Ref& obj ) ;
	
	void Store( IElement *element, Object& obj, const Ref& link ) ;

private :
	typedef std::map<Ref, IElement*> Map ;
	Map	m_map ;
	
	IFile *m_file ;
} ;

} // end of namespace

#endif
