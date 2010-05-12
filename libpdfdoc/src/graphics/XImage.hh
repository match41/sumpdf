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

/**	\file	XImage.hh
    \brief	definition the XImage class
    \date	May 11, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_XIMAGE_HH_EADER_INCLUDED__
#define __PDF_XIMAGE_HH_EADER_INCLUDED__

#include "graphics/Image.hh"
#include "page/XObject.hh"

namespace pdf {

///	brief description
/**	\internal
	The XImage class represents
*/
class XImage : public XObject, public Image
{
public :
	XImage( ) ;
} ;

} // end of namespace

#endif // XIMAGE_HH_
