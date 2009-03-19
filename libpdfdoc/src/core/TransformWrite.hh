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
	\file	TransformWrite.hh
	\brief	definition the TransformWrite class
	\date	Tue May 27 2008
	\author	Nestal Wan
*/

#ifndef __PDF_TRANSFORM_WRITE_HEADER_INCLUDED__
#define __PDF_TRANSFORM_WRITE_HEADER_INCLUDED__

#include "Object.hh"

#include <ostream>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
template <typename T, typename F>
class TransformWriter
{
public :
	TransformWriter( const T& obj, F func = F() )
		: m_obj( obj ), m_func( func )
	{
	}

	template <typename T, typename F>
	friend std::ostream& operator<<( std::ostream& os,
	                                 const TransformWriter<T, F>& tw )
	{
		return tw.m_func( os, tw.m_obj ) ;
	}

private :
	const T&	m_obj ;

	F	m_func ;
} ;

} // end of namespace

#endif
