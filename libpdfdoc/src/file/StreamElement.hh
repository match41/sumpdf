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
	\file	StreamElement.hh
	\brief	definition the StreamElement class
	\date	Sun Apr 5 2009
	\author	Nestal Wan
*/

#ifndef __PDF_STREAM_ELEMENT_HEADER_INCLUDED__
#define __PDF_STREAM_ELEMENT_HEADER_INCLUDED__

#include "IElement.hh"

#include "stream/Stream.hh"

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class StreamElement : public IElement, private Stream
{
public :
	StreamElement( ) ;

	void Init( Object& link, IElementSrc *src ) ;
	void Write( const Ref& link, IElementDest *dest ) const ;
	ElementList GetChildren( ) const ;
	
    using Stream::InStream ;
	
	using Stream::Reset ;

	using Stream::Type ;
	using Stream::Subtype ;
	
protected :
	using Stream::Dict ;
} ;

} // end of namespace

#endif
