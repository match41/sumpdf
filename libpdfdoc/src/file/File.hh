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
	\file	File.hh
	\brief	definition the File class
	\date	Fri Mar 21 2008
	\author	Nestal Wan
*/

#ifndef __PDF_FILE_HEADER_INCLUDED__
#define __PDF_FILE_HEADER_INCLUDED__

#include "IFile.hh"

#include "core/Dictionary.hh"

#include <iosfwd>
#include <vector>

namespace pdf {

class Dictionary ;
class Object ;
class Ref ;

/*!	\brief	PDF file class
	
	This class encapsulates the PDF file structure. Logicially it is a
	collection of PDF objects. These objects are stored in the PDF file with
	cross-links to each others. The offsets of these objects are stored in
	the end of the PDF file called the trailer. This class is responsible for
	decoding the file trailer dictionary and the cross reference table.
*/
class File : public IFile
{
public :
	File( std::istream *is ) ;
	File( std::ostream *os ) ;
	
	void WriteTrailer( const Ref& catalog,
	                   const std::string& producer	= std::string(),
	                   const std::string& creator	= std::string() ) ;
	Ref Root( ) const ;

	// override from IFile
	Object ReadObj( const Ref& obj ) ;
	Ref WriteObj( const Object& obj ) ;	
	Ref AllocLink( ) ;
	void WriteObj( const Object& obj, const Ref& link ) ;

private :
	void ReadXRef( std::size_t offset, Dictionary& trailer ) ;
	std::size_t ReadXRefOffset( ) ;

	bool ReadTailer( Dictionary& trailer, std::size_t& offset ) ;
	static std::istream& ReadLine( std::istream& is, std::string& line ) ;
	
	Object ReadStream( Dictionary& dict ) ;

	class ObjectWriter ;

private :
	std::vector<std::size_t>	m_objs ;

	std::istream	*m_in ;
	std::ostream	*m_out ;
		
	Dictionary		m_trailer ;
} ;

} // end of namespace

#endif
