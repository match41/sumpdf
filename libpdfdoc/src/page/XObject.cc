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
	\file	XObject.cc
	\brief	implementation the XObject class
	\date	Sat May 10 2008
	\author	Nestal Wan
*/

#include "XObject.hh"

#include "RealImage.hh"

#include "core/Dictionary.hh"
#include "core/Object.hh"

#include "file/ElementList.hh"
#include "file/IElementSrc.hh"
#include "file/IElementDest.hh"

#include <iostream>

namespace pdf {

XObject::XObject( )
{
}
/*
void XObject::Read( Stream& str, IElementSrc *repo )
{
	m_self.Swap( str ) ;
	
	// dereference
	Dictionary& dict = m_self.GetDictionary( ) ;
	for ( Dictionary::iterator i = dict.begin( ) ; i != dict.end( ) ; ++i )
	{
		if ( i->second.Type( ) == Object::ref )
			i->second = repo->ReadObj( i->second ) ;
	}
}
*/
void XObject::Init( Object& obj, IElementSrc *src )
{
	StreamElement::Init( obj, src ) ;

	std::cout << "this is an xobject: " << Dict()["Subtype"] << std::endl ;
}
/*
void XObject::Write( const Ref& link, IElementDest *repo ) const
{
	repo->WriteObj( m_self, link ) ;
}

ElementList XObject::GetChildren( ) const
{
	return ElementList( ) ;
}
*/

template <> XObject* CreateNewElement( const Object& obj, IElementSrc *src )
{
	assert( src != 0 ) ;

	Name type = obj.As<Stream>().Subtype( ) ;
	if ( type.Str() == "Image" )
		return new RealImage ; 
	
	else
		return new XObject ;
}

} // end of namespace
