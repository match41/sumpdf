/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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

/**
	\file	ResourcePool.h
	\brief	definition the ResourcePool class
	\date	Dec 26, 2009
	\author	nestal
*/


#ifndef __PDF_RESOURCEPOOL_HEADER_INCLUDED__
#define __PDF_RESOURCEPOOL_HEADER_INCLUDED__

#include "RefObjMap.hh"
#include "core/ObjWrapper.hh"

namespace pdf {

class BaseFont ;
class Object ;

typedef RefObjMap<BaseFont>			FontPool ;
typedef RefObjMap<ObjWrapper>		ObjectPool ;

struct ResourcePool
{
	FontPool	fonts ;
	ObjectPool	objs ;
} ;

} // end of namespace

#endif // RESOURCEPOOL_H_
