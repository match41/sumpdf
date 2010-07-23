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
	\file	RealFile.hh
	\brief	definition the RealFile class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#ifndef __PDF_REAL_FILE_HEADER_INCLUDED__
#define __PDF_REAL_FILE_HEADER_INCLUDED__

#include "File.hh"

#include "ElementPool.hh"

#include "core/Dictionary.hh"

#include <iosfwd>
#include <vector>

namespace pdf {

class Dictionary ;
class Object ;
class Ref ;
class Stream ;

/*!	\brief	PDF file class
	
	This class encapsulates the PDF file structure. Logically it is a
	collection of PDF objects. These objects are stored in the PDF file with
	cross-links to each others. The offsets of these objects are stored in
	the end of the PDF file called the trailer. This class is responsible for
	decoding the file trailer dictionary and the cross reference table.
*/
class RealFile : public File
{
public :
	explicit RealFile( std::istream *is ) ;
	explicit RealFile( std::ostream *os ) ;
	
	void WriteTrailer( const Ref& catalog, const Ref& info ) ;
	Ref Root( ) const ;
	Ref DocInfo( ) const ;

	// override from IFile
	Object ReadObj( const Ref& obj ) ;
	
	Ref WriteObj( const Object& obj ) ;	
	Ref AllocLink( ) ;
	void WriteObj( const Object& obj, const Ref& link ) ;
	ElementPool* Pool( ) ;
	
	// type specific read function
	void ReadType( const Ref& link, Dictionary& dict ) ;
	void ReadType( const Ref& link, Array& value ) ;
	void ReadType( const Ref& link, int& value ) ;
	void ReadType( const Ref& link, double& value ) ;
	void ReadType( const Ref& link, bool& value ) ;
	void ReadType( const Ref& link, Name& value ) ;
	void ReadType( const Ref& link, std::string& value ) ;
	void ReadType( const Ref& link, Ref& value ) ;
	void ReadType( const Ref& link, Stream& value ) ;
	void ReadType( const Ref& link, Object& obj ) ;

private :
	template <typename T>
	void BasicRead( const Ref& link, T& result ) ;

	void ReadXRef( std::size_t offset, Dictionary& trailer ) ;
	void ReadXRefTable( Dictionary& trailer ) ;
	void ReadXRefStream( std::size_t offset, Dictionary& trailer ) ;
	std::size_t ReadXRefOffset( ) ;
	void ReadNextObj( Object& obj ) ;

	bool ReadTrailer( Dictionary& trailer, std::size_t& offset ) ;
	static std::istream& ReadLine( std::istream& is, std::string& line ) ;
	
	Stream ReadStream( Dictionary& dict ) ;

	class	ObjectWriter ;

private :
	std::vector<std::size_t>	m_objs ;

	std::istream	*m_in ;
	std::ostream	*m_out ;
		
	Dictionary		m_trailer ;
	ElementPool	m_pool ;
} ;

} // end of namespace

#endif
