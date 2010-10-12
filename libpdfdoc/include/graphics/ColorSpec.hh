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

/**	\file	ColorSpec.hh
    \brief	definition the ColorSpec class
    \date	Oct 12, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_COLORSPEC_HH_EADER_INCLUDED__
#define __PDF_COLORSPEC_HH_EADER_INCLUDED__

#include <string>

namespace pdf {

namespace gfx
{
	///	brief description
	/**	The ColorSpec enum represents how to specify colors. It is different
		from a color space which is more complicated.
	*/
	enum ColorSpec
	{
		rgb,
		gray,
		cmyk,
		
		none
	} ;
}

using gfx::ColorSpec ;

ColorSpec ParseSpec( const std::string& name ) ;
const std::string& SpecName( ColorSpec spec ) ;

} // end of namespace

#endif // COLORSPEC_HH_
