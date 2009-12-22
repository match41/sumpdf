/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
 \***************************************************************************/

/**
 \file	SfntFile.hh
 \brief	definition the SfntFile class
 \date	Dec 22, 2009
 \author	nestal
 */

#ifndef __PDF_SFNTFILE_HEADER_INCLUDED__
#define __PDF_SFNTFILE_HEADER_INCLUDED__

#include <memory>

namespace ft
{
	class Face ;
}

namespace tex {

class SfntFile
{
public:
	SfntFile( const ft::Face& face ) ;
	~SfntFile( ) ;

private :
	struct Impl ;
	Impl	*m_sfnt ;
} ;

} // end of namespace

#endif // SFNTFILE_HH_
