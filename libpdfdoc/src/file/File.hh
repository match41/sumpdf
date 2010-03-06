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
	\file	File.hh
	\brief	definition the File class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#ifndef __PDF_FILE_HEADER_INCLUDED__
#define __PDF_FILE_HEADER_INCLUDED__

#include <string>

namespace pdf {

class Array ;
class Name ;
class Object ;
class Ref ;
class ElementPool ;
class Dictionary ;
class Stream ;

/*!	\brief	PDF file interface

	This class represents the interface of PDF file structure. It allows the
	caller to read PDF core objects base on their reference.
*/
class File
{
protected :
	virtual ~File( ) ;

public :
	virtual Object ReadObj( const Ref& obj ) = 0 ;
	virtual Ref WriteObj( const Object& obj ) = 0 ;

	virtual Ref AllocLink( ) = 0 ;
	virtual void WriteObj( const Object& obj, const Ref& link ) = 0 ;

	virtual ElementPool* Pool( ) = 0 ;

	// type specific read function
	virtual void ReadType( const Ref& link, Dictionary& dict ) = 0 ;
	virtual void ReadType( const Ref& link, Array& array ) = 0 ;
	virtual void ReadType( const Ref& link, int& value ) = 0 ;
	virtual void ReadType( const Ref& link, double& value ) = 0 ;
	virtual void ReadType( const Ref& link, bool& value ) = 0 ;
	virtual void ReadType( const Ref& link, Name& value ) = 0 ;
	virtual void ReadType( const Ref& link, std::string& value ) = 0 ;
	virtual void ReadType( const Ref& link, Ref& value ) = 0 ;
	virtual void ReadType( const Ref& link, Stream& value ) = 0 ;
	virtual void ReadType( const Ref& link, Object& obj ) = 0 ;
} ;

} // end of namespace

#endif
