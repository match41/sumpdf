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
	\file	RawElement.hh
	\brief	definition the RawElement class
	\date	Sat Apr 18 2009
	\author	Nestal Wan
*/

#ifndef __PDF_RAW_ELEMENT_HEADER_INCLUDED__
#define __PDF_RAW_ELEMENT_HEADER_INCLUDED__

#include "IElement.hh"

#include "core/Object.hh"
#include "core/Ref.hh"

#include <map>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class RawElement : public IElement
{
public :
	RawElement( ) ;

	void Init( Object& obj, IElementSrc *src ) ;
	void Write( const Ref& link, IElementDest *dest ) const ;

	ElementList GetChildren( ) const ;

private :
	void ReadChild( Object& obj, IElementSrc *src ) ;
	void WriteChild( const Object& obj, IElementDest *dest ) const ;

private :
	Object	m_self ;
	
	std::map<Ref, IElement*>	m_children ;
} ;

} // end of namespace

#endif
