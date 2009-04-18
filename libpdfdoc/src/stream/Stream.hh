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
#include <iosfwd>

namespace pdf {

class Dictionary ;
class Name ;
class Ref ;
class Object ;

/*!	\brief	PDF stream object
	
	This class represents a PDF stream object. A stream is a number of bytes
	stored in a PDF file. It may be encoded by a number of filters.
	Unlike other PDF object classes, this class does not support operator>>()
	and operator<<(). It is because although the Stream classes represents one
	of the fundamental data types, it may depend on references to other objects
	such as its length. It cannot be fully extracted by operator>>() and it
	can't be written completely by operator<<(). The constructor and the
	Write() function will perform reading and writing from and to file.
*/
class Stream
{
public :
	explicit Stream( const std::string& str = std::string() ) ;
	Stream( std::vector<unsigned char>& data, const Object& filter ) ;
	Stream( std::streambuf *file, std::streamoff offset,
	        const Dictionary& dict ) ;
	~Stream( ) ;

	bool operator==( const Stream& str ) const ;

	Dictionary MakeDictWithLength( const Ref& length_link ) const ;
    std::size_t WriteData( std::streambuf *buf ) const ;

	void Swap( Stream& str ) ;
	std::istream& InStream( ) const ;
	
	void Reset( ) const ;

	Name Type( ) const ;
	Name Subtype( ) const ;

protected :
	const Dictionary& Dict( ) const ;
	Dictionary& Dict( ) ;

private :
	void ApplyFilter( const Object& filter ) ;
	void CreateFilter( const Name& filter ) ;

private :
	struct Impl ;
	
	/// pointer to implementation. It uses shared_ptr for copy-on-write
	///	reference counting.
	boost::shared_ptr<Impl>	m_impl ;
} ;

} // end of namespace

namespace std
{
	void swap( pdf::Stream& s1, pdf::Stream& s2 ) ;
}

#endif
