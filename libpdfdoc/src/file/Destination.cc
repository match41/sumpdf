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

#include "IFile.hh"
#include "ResourcePool.hh"
#include "page/RealPage.hh"

#include <iostream>

namespace pdf {

/**	constructor
*/
Destination::Destination( )
	: m_page( 0 )
{
}

void Destination::Read( Array& array, IFile *file )
{
	m_options.swap( array ) ;
	if ( m_options.size() > 1 )
	{
		PagePool *pool = &file->Pool()->pages ;
		Ref page_link = m_options[0].As<Ref>() ;
		m_page = dynamic_cast<RealPage*>( pool->Find( page_link ) ) ;
std::cout << "link to page: " << m_page << std::endl ;
	}
}

Ref Destination::Write( IFile *file ) const
{
	Array self( m_options ) ;
	PagePool *pool = &file->Pool()->pages ;
	self[0] = pool->Find( m_page ) ;
	return file->WriteObj( self ) ;
}

} // end of namespace
