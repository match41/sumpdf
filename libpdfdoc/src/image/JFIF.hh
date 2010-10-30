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

/**	\file	JFIF.hh
    \brief	definition the JFIF class
    \date	Oct 30, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_JFIF_HH_EADER_INCLUDED__
#define __PDF_JFIF_HH_EADER_INCLUDED__

#include <iosfwd>
#include <vector>

namespace img {

///	brief description
/**	\internal
	The JFIF class represents
*/
class JFIF
{
public :
	explicit JFIF( std::streambuf *src, std::vector<unsigned char> *buf = 0 ) ;
	
	std::size_t Size( ) const ;

private :
	unsigned short ReadBigEndlian( ) const ;

private :
	std::streambuf				*m_src ;
	std::vector<unsigned char>	*m_buf ;
} ;

} // end of namespace

#endif // JFIF_HH_
