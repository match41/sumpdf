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
	\file	IFile.hh
	\brief	definition the IFile class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#ifndef __PDF_IFILE_HEADER_INCLUDED__
#define __PDF_IFILE_HEADER_INCLUDED__

namespace pdf {

class Object ;
class Ref ;
class StreamFilter ;

/*!	\brief	brief description

	this class represents
*/
class IFile
{
protected :
	virtual ~IFile( ) ;

public :
	virtual Object ReadObj( const Ref& obj ) = 0 ;
	virtual Ref WriteObj( const Object& obj ) = 0 ;

	virtual Ref AllocLink( ) = 0 ;
	virtual void WriteObj( const Object& obj, const Ref& link ) = 0 ;
} ;

} // end of namespace

#endif
