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
	\file	StreamElement.cc
	\brief	implementation the StreamElement class
	\date	Sun Apr 5 2009
	\author	Nestal Wan
*/

#include "StreamElement.hh"

#include "IElementDest.hh"
#include "ElementList.hh"

#include "core/Object.hh"

namespace pdf {

StreamElement::StreamElement( )
{
}

void StreamElement::Init( Object& obj, ElementReader * )
{
	Swap( obj.As<Stream>() ) ;
}

void StreamElement::Write( const Ref& link, IElementDest *dest ) const
{
	dest->WriteObj( static_cast<const Stream&>(*this), link ) ;
}

ElementList StreamElement::GetChildren( ) const
{
	return ElementList( ) ;
}

} // end of namespace
