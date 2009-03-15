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
	\file	ElementDest.hh
	\brief	definition the ElementDest class
	\date	Sun Apr 13 2008
	\author	Nestal Wan
*/

#ifndef __PDF_ELEMENT_DEST_HEADER_INCLUDED__
#define __PDF_ELEMENT_DEST_HEADER_INCLUDED__

#include "IElementDest.hh"

#include "core/Ref.hh"

#include <map>

namespace pdf {

class IElement ;
class IFile ;

/*!	\brief	brief description
	
	this class represents
*/
class ElementDest : public IElementDest
{
public :
	ElementDest( IFile *output ) ;

	Ref WriteObj( const Object& obj ) ;
	
	Ref AllocLink( ) ;
	void WriteObj( const Object& obj, const Ref& link ) ;
	
	Ref Write( const IElement *element ) ;

private :
	void Dereference( Object& obj ) ;

private :
	typedef std::map<const IElement*, Ref> Map ;
	Map	m_map ;
	
	IFile *m_file ;
} ;

} // end of namespace

#endif
