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
	\file	RawElement.cc
	\brief	implementation the RawElement class
	\date	Sat Apr 18 2009
	\author	Nestal Wan
*/

#include "RawElement.hh"

#include "ElementList.hh"
#include "IElementDest.hh"
#include "IElementSrc.hh"

#include "core/TraverseObject.hh"

#include <boost/bind.hpp>

namespace pdf {

RawElement::RawElement( )
{
}

void RawElement::Init( Object& obj, IElementSrc *src )
{
	m_self.Swap( obj ) ;

	ForEachObj( m_self, boost::bind( &RawElement::ReadChild, this, _1, src ) ) ;
}

void RawElement::ReadChild( Object& obj, IElementSrc *src )
{
	if ( obj.IsType<Ref>() )
		m_children[obj.As<Ref>()] = src->Read<RawElement>( obj.As<Ref>() ) ;
}

void RawElement::Write( const Ref& link, IElementDest *dest ) const
{
	dest->WriteObj( m_self, link ) ;
}

void RawElement::WriteChild( const Object& obj, IElementDest *dest ) const
{
}

ElementList RawElement::GetChildren( ) const
{
	return ElementList() ;
}

} // end of namespace
