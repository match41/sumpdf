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
	\file	MockFile.hh
	\brief	definition the MockFile class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#ifndef __PDFUT_MOCK_FILE_HEADER_INCLUDED__
#define __PDFUT_MOCK_FILE_HEADER_INCLUDED__

#include "file/File.hh"

#include "file/ElementPool.hh"

#include "core/Object.hh"
#include "core/Ref.hh"

#include <map>

namespace pdfut {

using namespace pdf ;

/*!	\brief	brief description
	
	this class represents
*/
class MockFile : public pdf::File
{
private :
	std::map<pdf::Ref, pdf::Object>	m_map ;

	std::size_t	m_counter ;
	
	pdf::ElementPool	m_pool ;

public :
	MockFile( ) ;
	
	void AddObj( const pdf::Ref& link, const pdf::Object& obj ) ;
	const pdf::Object& Find( const pdf::Ref& link ) const ;
	void AddText( const Ref& link, const std::string& str ) ;
	void ClearPool( ) ;
	
	pdf::Object ReadObj( const pdf::Ref& obj ) ;
	pdf::Ref WriteObj( const pdf::Object& obj ) ;
	pdf::Ref AllocLink( ) ;
	void WriteObj( const pdf::Object& obj, const pdf::Ref& link ) ;
	pdf::ElementPool* Pool( ) ;

	// type specific read function
	void ReadType( const Ref& link, Dictionary& dict ) ;
	void ReadType( const Ref& link, Array& array ) ;
	void ReadType( const Ref& link, int& value ) ;
	void ReadType( const Ref& link, double& value ) ;
	void ReadType( const Ref& link, bool& value ) ;
	void ReadType( const Ref& link, Name& value ) ;
	void ReadType( const Ref& link, std::string& value ) ;
	void ReadType( const Ref& link, Ref& value ) ;
	void ReadType( const Ref& link, Stream& value ) ;
	void ReadType( const Ref& link, Object& obj ) ;
} ;

} // end of namespace

#endif
