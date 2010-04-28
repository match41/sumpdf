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

/**	\file	Type1Encoding.hh
    \brief	definition the Type1Encoding class
    \date	Apr 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_TYPE1ENCODING_HH_EADER_INCLUDED__
#define __PDF_TYPE1ENCODING_HH_EADER_INCLUDED__

// base class headers
#include "BaseEncoding.hh"

#include <wchar.h>

struct FT_FaceRec_ ;

namespace pdf {

///	brief description
/**	\internal
	The Type1Encoding class represents
*/
class Type1Encoding : public BaseEncoding
{
public :
	explicit Type1Encoding( FT_FaceRec_ *face ) ;
} ;

} // end of namespace

#endif // TYPE1ENCODING_HH_
