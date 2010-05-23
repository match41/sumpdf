/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
	\file	MockStreamFilter.cc
	\brief	definition the MockStreamFilter class
	\date	Dec 22, 2009
	\author	Nestal Wan
*/

#include "MockStreamFilter.hh"

#include "core/Object.hh"

namespace pdf {

MockStreamFilter::MockStreamFilter(
	std::auto_ptr<StreamFilter>	inner,
	const Name&					filter )
	: m_inner( inner ),
	  m_filter( filter )
{
}

std::size_t MockStreamFilter::Read( unsigned char *data, std::size_t size )
{
	return m_inner.get() != 0 ? m_inner->Read( data, size ) : 0 ;
}

std::size_t MockStreamFilter::Write(
	const unsigned char *data,
	std::size_t size )
{
	return m_inner.get() != 0 ? m_inner->Write( data, size ) : 0 ;
}

void MockStreamFilter::Flush( )
{
	if ( m_inner.get() != 0 )
		m_inner->Flush( ) ;
}

void MockStreamFilter::Rewind( )
{
	if ( m_inner.get() != 0 )
		m_inner->Rewind( ) ;
}

std::size_t MockStreamFilter::Length( ) const
{
	return m_inner.get() != 0 ? m_inner->Length() : 0 ;
}

Object MockStreamFilter::NameChain( ) const
{
	// TODO: need to use array for multiple filter names
	return m_filter.empty() ? Object( ) : Object( m_filter ) ;
}

StreamFilter* MockStreamFilter::Clone( ) const
{
	std::auto_ptr<StreamFilter> f( m_inner->Clone() ) ;
	return new MockStreamFilter( f, m_filter ) ;
}

StreamFilter* MockStreamFilter::GetInner( )
{
	return m_inner.get() != 0 ? m_inner.get() : this ;
}

Name MockStreamFilter::RawFormat( ) const
{
	return m_filter ;
}

} // end of namespace
