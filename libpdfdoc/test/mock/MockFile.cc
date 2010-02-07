/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	MockFile.cc
	\brief	implementation the MockFile class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#include "MockFile.hh"

MockFile::MockFile( )
	: m_counter( 0 )
{
}

void MockFile::AddObj( const pdf::Ref& link, const pdf::Object& obj )
{
	m_map[link] = obj ;
}

pdf::Object MockFile::ReadObj( const pdf::Ref& obj )
{
	return m_map[obj] ;
}

pdf::Ref MockFile::WriteObj( const pdf::Object& obj )
{
	pdf::Ref link = AllocLink( ) ;
	WriteObj( obj, link ) ;
	return link ;
}

pdf::Ref MockFile::AllocLink( )
{
	return pdf::Ref( m_counter++, 0 ) ;
}

void MockFile::WriteObj( const pdf::Object& obj, const pdf::Ref& link )
{
	m_map[link] = obj ;
}

const pdf::Object& MockFile::Find( const pdf::Ref& link ) const
{
	std::map<pdf::Ref, pdf::Object>::const_iterator i = m_map.find( link ) ;
	return i != m_map.end( ) ? i->second : pdf::Object::NullObj() ;
}

pdf::ElementPool* MockFile::Pool( )
{
	return &m_pool ;
}

void MockFile::ClearPool( )
{
	m_pool.Clear() ;
}
