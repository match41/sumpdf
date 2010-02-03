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

/**	\file	FontType.hh
    \brief	definition the FontType class
    \date	Jan 28, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTTYPE_HH_EADER_INCLUDED__
#define __PDF_FONTTYPE_HH_EADER_INCLUDED__

struct FT_FaceRec_ ;

namespace pdf {

namespace font
{
	///	brief description
	/**	The Type enum represents an enumeration of font types.
	*/
	enum Type { truetype, type1, mmtype1, type3, type0, openType, unknown } ;
	
	Type GetType( FT_FaceRec_ *face ) ;
}

} // end of namespace

#endif // FONTTYPE_HH_
