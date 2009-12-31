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
	\file	CompleteObj.cc
	\brief	implementation of the CompleteObj class
	\date	Jan 1, 2010
	\author	Nestal Wan
*/

#include "CompleteObj.hh"

#include "IFile.hh"

#include "core/Dictionary.hh"
#include "core/Array.hh"

namespace pdf {

/**	constructor
*/
CompleteObj::CompleteObj( )
{
}

void CompleteObj::Read( Dictionary& dict, IFile *file )
{
	assert( file != 0 ) ;

	m_obj = Dictionary( ) ;
	m_obj.Swap( dict ) ;
	
	file->ReadObjectLinks( m_obj, m_refs ) ;
}

void CompleteObj::Read( Array& array, IFile *file )
{
	assert( file != 0 ) ;

	m_obj = Array( ) ;
	m_obj.Swap( array ) ;
	
	file->ReadObjectLinks( m_obj, m_refs ) ;
}

Ref CompleteObj::Write( IFile *file ) const
{
	// TODO: implement this
	return Ref( ) ;
}

} // end of namespace
