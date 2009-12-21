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

/**
 \file	MockStreamFilter.cpp
 \brief	definition the MockStreamFilter class
 \date	Dec 22, 2009
 \author	nestal
 */

#include "MockStreamFilter.hh"

#include "core/Object.hh"

namespace pdf {

MockStreamFilter::MockStreamFilter( )
{
}

std::size_t MockStreamFilter::Read( unsigned char *data, std::size_t size )
{
	return 0 ;
}

std::size_t MockStreamFilter::Write(
	const unsigned char *data,
	std::size_t size )
{
	return 0 ;
}

void MockStreamFilter::Flush( )
{
}

void MockStreamFilter::Rewind( )
{
}

std::size_t MockStreamFilter::Length( ) const
{
	return 0 ;
}

Object MockStreamFilter::GetFilterName( ) const
{
	return Object( ) ;
}

StreamFilter* MockStreamFilter::Clone( ) const
{
	return new MockStreamFilter( *this ) ;
}

StreamFilter* MockStreamFilter::GetInner( )
{
	return this ;
}

} // end of namespace
