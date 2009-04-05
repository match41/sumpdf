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
	\file	XObject.hh
	\brief	definition the XObject class
	\date	Sat May 10 2008
	\author	Nestal Wan
*/

#ifndef __PDF_XOBJECT_HEADER_INCLUDED__
#define __PDF_XOBJECT_HEADER_INCLUDED__

#include "file/StreamElement.hh"

#include "file/IElementSrc.hh"

namespace pdf {

class Stream ;

/*!	\brief	PDF external objects
	
	According to the PDF specification: "An external object (commonly called an
	XObject) is a graphics object whose contents are defined by a self-contained
	content stream."
*/
class XObject : public StreamElement
{
public :
	XObject( ) ;

	void Init( Object& link, IElementSrc *repo ) ;
/*	void Read( Stream& str, IElementSrc *repo ) ;
	void Write( const Ref& link, IElementDest *repo ) const ;

	ElementList GetChildren( ) const ;*/
} ;

template <> XObject* CreateNewElement( const Object& link, IElementSrc * ) ;

} // end of namespace

#endif
