/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
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
 ***************************************************************************/

/*!
	\file	RealImage.hh
	\brief	definition the Image class
	\date	Thu Oct 16 2008
	\author	Nestal Wan
*/

#ifndef __PDF_REAL_IMAGE_HEADER_INCLUDED__
#define __PDF_REAL_IMAGE_HEADER_INCLUDED__

#include "XObject.hh"

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class RealImage : public XObject
{
public :
	RealImage( const std::string& filename ) ;

	
} ;

} // end of namespace

#endif