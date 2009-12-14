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
	\file	pdfobj.cc
	\brief	definition the pdfobj class
	\date	Dec 12, 2009
	\author	nestal
*/

#include "pdfobj.hh"

#include "core/Object.hh"
#include "stream/Stream.hh"

#include <cassert>

using namespace pdf ;

struct pdf_obj
{
	Object	*obj ;
} ;

pdf_obj    *pdf_new_stream        (int flags)
{
	pdf_obj *obj	= new pdf_obj ;
	obj->obj		= new Object( Stream( ) ) ;
	return obj ;
}
