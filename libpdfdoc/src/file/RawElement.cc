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
#include "ElementReader.hh"

#include "core/TraverseObject.hh"

#include <boost/bind.hpp>

#include <cassert>

namespace pdf {

RawElement::RawElement( )
{
}

void RawElement::Init( Object& obj, ElementReader *src )
{
	assert( src != 0 ) ;

	m_self.Swap( obj ) ;

	ForEachObj( m_self, boost::bind( &RawElement::ReadChild, this, _1, src ) ) ;
}

void RawElement::ReadChild( Object& obj, ElementReader *src )
{
	assert( src != 0 ) ;

	if ( obj.Type() == Object::ref )
		m_children[obj.As<Ref>()] = src->Read( obj.As<Ref>() ) ;
}

void RawElement::Write( const Ref& link, IElementDest *dest ) const
{
	assert( dest != 0 ) ;

	// constant function can't update self
	Object temp = m_self ;
	
	ForEachObj( temp, boost::bind( &RawElement::WriteChild, this, _1, dest ) ) ;
	
	dest->WriteObj( temp, link ) ;
}

void RawElement::WriteChild( Object& obj, IElementDest *dest ) const
{
	assert( dest != 0 ) ;
	if ( obj.Type() == Object::ref )
	{
		assert( m_children.find( obj.As<Ref>() ) != m_children.end() ) ;
		
		IElement *e = m_children[obj.As<Ref>()] ;
		assert( e != 0 ) ;
		
		obj = dest->Write( e ) ;
	}
}

ElementList RawElement::GetChildren( ) const
{
	return ElementList() ;
}

Object& RawElement::Get( )
{
	return m_self ;
}

const Object& RawElement::Get( ) const
{
	return m_self ;
}

} // end of namespace
