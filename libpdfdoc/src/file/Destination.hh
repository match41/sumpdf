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
    \file	Destination.hh
    \brief	definition the Destination class
    \date	Jan 2, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_DESTINATION_HEADER_INCLUDED__
#define __PDF_DESTINATION_HEADER_INCLUDED__

#include "core/Array.hh"

namespace pdf {

class Array ;
class File ;
class RealPage ;
class Ref ;

/**	\brief	brief description

	The Destination class represent
*/
class Destination
{
public :
	Destination( ) ;
	~Destination( ) ;
	
	void Read( Array& array, File *file ) ;
	Ref Write( File *file ) const ;

private :
	RealPage	*m_page ;
	Array		m_options ;
} ;

} // end of namespace

#endif // DESTINATION_HH_
