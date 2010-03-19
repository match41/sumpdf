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
	\file	CreateFont.cc
	\brief	definition the CreateFont() function
	\date	Sun Mar 8 2009
	\author	Nestal Wan
*/

#include "SimpleFont.hh"
#include "CompositeFont.hh"
#include "FontException.hh"

#include "file/DictReader.hh"

namespace pdf
{

BaseFont* CreateFont( DictReader& obj, FontDb *db )
{
	Name subtype = obj["Subtype"] ;
	
	if ( subtype == Name("Type0") )
		return new CompositeFont( obj, db ) ;
	
	else
		return new SimpleFont( obj, db ) ;
}

}
