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
	\file	MockFile.hh
	\brief	definition the MockFile class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_MOCK_FILE_HEADER_INCLUDED__
#define __PDFUT_MOCK_FILE_HEADER_INCLUDED__

#include "file/IFile.hh"

#include "core/Object.hh"
#include "core/Ref.hh"

#include <map>

/*!	\brief	brief description
	
	this class represents
*/
class MockFile : public pdf::IFile
{
private :
	std::map<pdf::Ref, pdf::Object>	m_map ;

	std::size_t	m_counter ;
	
	static const pdf::Object m_null ;

public :
	MockFile( ) ;
	
	void AddObj( const pdf::Ref& link, const pdf::Object& obj ) ;
	const pdf::Object& Find( const pdf::Ref& link ) const ;
	
	pdf::Object ReadObj( const pdf::Ref& obj ) ;
	pdf::Ref WriteObj( const pdf::Object& obj ) ;
	pdf::Ref AllocLink( ) ;
	void WriteObj( const pdf::Object& obj, const pdf::Ref& link ) ;
} ;

#endif
