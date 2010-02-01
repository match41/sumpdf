/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	libpdfdoc.cc
	\brief	implementation the export functions for the libpdfdoc DLL
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#include "libpdfdoc.hh"

#include "RealDoc.hh"

namespace pdf {

Doc*  CreateDoc( )
{
	return new RealDoc ;
}

const char* Version( )
{
	static const char version[] = VERSION ;
	return version ;
}

} // end of namespace
