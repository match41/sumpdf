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

/**	\file	ReadStream.hh
    \brief	definition the ReadStream class
    \date	Apr 10, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_READSTREAM_HH_EADER_INCLUDED__
#define __PDF_READSTREAM_HH_EADER_INCLUDED__

#include <cstddef>

namespace pdf {

///	brief description
/**	\internal
	The ReadStream class represents
*/
class ReadStream
{
public :
	ReadStream( const unsigned char *p, std::size_t size ) ;

	template <typename T>
	ReadStream& operator>>( T& v ) ;
	
private :
	const unsigned char	*m_ptr ;
	std::size_t			m_size ;
} ;

} // end of namespace

#endif // READSTREAM_HH_
