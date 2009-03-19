/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
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
 ***************************************************************************/

/*!
	\file	ElementSrc.hh
	\brief	definition the ElementSrc class
	\date	Sun Mar 30 2008
	\author	Nestal Wan
*/

#ifndef __PDF_ELEMENT_REPO_HEADER_INCLUDED__
#define __PDF_ELEMENT_REPO_HEADER_INCLUDED__

#include "IElementSrc.hh"

#include "core/Ref.hh"

#include <map>

namespace pdf {

class IElement ;
class IFile ;

/*!	\brief	A source for elements
	
	this class represents
*/
class ElementSrc : public IElementSrc
{
public :
	ElementSrc( IFile *file ) ;

	Object ReadObj( const Ref& obj, bool deref = false ) ;

private :
	IElement* Init( IElement *element, const Ref& link ) ;
	IElement* Find( const Ref& link ) ;
	
	void Dereference( Object& obj ) ;
	
private :
	typedef std::map<Ref, IElement*> Map ;
	Map	m_map ;
	
	IFile *m_file ;
} ;

} // end of namespace

#endif
