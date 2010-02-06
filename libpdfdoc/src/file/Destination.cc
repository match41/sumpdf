/***************************************************************************\
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2.                              *
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

/**
	\file	Destination.cc
	\brief	implementation of the Destination class
	\date	Jan 2, 2010
	\author	Nestal Wan
*/

#include "Destination.hh"

#include "File.hh"
#include "ElementPool.hh"
#include "page/RealPage.hh"

#include <iostream>

namespace pdf {

/**	constructor
*/
Destination::Destination( )
	: m_page( 0 )
{
}

Destination::~Destination( )
{
	if ( m_page != 0 )
		m_page->Release( ) ;
}

void Destination::Read( Array& array, File *file )
{
	m_options.swap( array ) ;
	if ( m_options.size() > 1 )
	{
		ElementPool *pool = file->Pool( ) ;
		Ref page_link = m_options[0].As<Ref>() ;
		m_page = pool->Find<RealPage>( page_link ) ;
	}
}

Ref Destination::Write( File *file ) const
{
	Array self( m_options ) ;
//	PagePool *pool = &file->Pool()->pages ;
//	self[0] = pool->Find( m_page ) ;
	return file->WriteObj( self ) ;
}

} // end of namespace
