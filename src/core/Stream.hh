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
	\file	Stream.hh
	\brief	definition the Stream class
	\date	Thu Mar 13 2008
	\author	Nestal Wan
*/

#ifndef __PDF_STREAM_HEADER_INCLUDED__
#define __PDF_STREAM_HEADER_INCLUDED__

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace pdf {

class Dictionary ;
class Name ;

/*!	\brief	PDF stream object
	
	This class represents a PDF stream object. A stream is a number of bytes
	stored in a PDF file. It may be encoded by a number of filters.
*/
class Stream
{
public :
	Stream( ) ;
	Stream( const std::string& str ) ;
	Stream( std::vector<unsigned char>& data, const Dictionary& dict ) ;
	Stream( std::streambuf *file, std::streamoff offset,
	        const Dictionary& dict ) ;
	~Stream( ) ;

	friend std::ostream& operator<<( std::ostream& os, const Stream& str ) ;

	const unsigned char* Data( ) const ;
	std::size_t Size( ) const ;

	bool operator==( const Stream& str ) const ;

	const Dictionary& GetDictionary( ) const ;
	Dictionary& GetDictionary( ) ;

	bool IsEmpty( ) const ;

	void Swap( Stream& str ) ;

private :
	void Inflate( ) ;

	void ApplyFilter( const Name& filter ) ;

private :
	struct Impl ;
	boost::shared_ptr<Impl>	m_impl ;
} ;

} // end of namespace

#endif
