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

/**	\file	AdobeCMap.hh
    \brief	definition the AdobeCMap class
    \date	Apr 21, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_ADOBECMAP_HH_EADER_INCLUDED__
#define __PDF_ADOBECMAP_HH_EADER_INCLUDED__

#include <iosfwd>

namespace pdf {

///	brief description
/**	\internal
	The AdobeCMap class represents
*/
class AdobeCMap
{
public :
	AdobeCMap( std::istream& is ) ;

private :
	void ReadBFRange( std::istream& is, std::size_t count ) ;
} ;

} // end of namespace

#endif // ADOBECMAP_HH_
