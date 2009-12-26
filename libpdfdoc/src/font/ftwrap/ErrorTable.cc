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

/*!
	\file	ErrorTable.cc
	\brief	implementation the ErrorTable class
	\date	Tue Mar 17 2009
	\author	Nestal Wan
*/

#include "ErrorTable.hh"

#include "util/Util.hh"

#include <cassert>

// build freetype error strings table
#include <ft2build.h>
#include FT_FREETYPE_H
#define FT_ERROR_START_LIST     {
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_END_LIST       { 0, 0 } };

#undef __FTERRORS_H__

namespace
{
	const struct ErrorTable
	{
		int          code;
		const char*  msg;
	} ft_errors[] =
	#include FT_ERRORS_H 
	
	// add semi-colon to satisfy kdevelop's syntax highlight
	;
}

namespace ft {

const char* LookupError( int error )
{
	assert( error >= 0 ) ;
	assert( error < static_cast<int>( pdf::Count( ft_errors ) ) ) ;
	return ft_errors[error].msg ;
}

} // end of namespace
