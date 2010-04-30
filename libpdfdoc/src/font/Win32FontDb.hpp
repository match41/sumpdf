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

/**	\file	Win32FontDb.hh
    \brief	definition the Win32FontDb class
    \date	Feb 14, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_WIN32FONTDB_HH_EADER_INCLUDED__
#define __PDF_WIN32FONTDB_HH_EADER_INCLUDED__

#include "BasicFontDb.hh"

namespace pdf {

///	brief description
/**	\internal
	The Win32FontDb class represents
*/
class Win32FontDb : public BasicFontDb
{
public :
	Win32FontDb( ) ;

	std::vector<unsigned char> FindFont( 
		const std::string&	base_name,
		font::Weight		weight,
		font::Slant			slant,
		font::Width			width ) ;
} ;

} // end of namespace

#endif // WIN32FONTDB_HH_
