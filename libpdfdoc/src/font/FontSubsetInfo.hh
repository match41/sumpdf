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

/**	\file	FontSubsetInfo.hh
    \brief	definition the FontSubsetInfo class
    \date	Apr 12, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTSUBSETINFO_HH_EADER_INCLUDED__
#define __PDF_FONTSUBSETINFO_HH_EADER_INCLUDED__

#include <vector>
#include <wchar.h>

namespace pdf {

class BaseFont ;

///	brief description
/**	\internal
	The FontSubsetInfo class represents
*/
class FontSubsetInfo
{
protected :
	~FontSubsetInfo( ) ;

public :
	virtual std::vector<wchar_t> GetUsedChars( const BaseFont *f ) const = 0;
} ;

} // end of namespace

#endif // FONTSUBSETINFO_HH_
