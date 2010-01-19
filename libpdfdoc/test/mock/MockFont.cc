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

/**	\file	MockFont.cc
	\brief	implementation of the MockFont class
	\date	Jan 18, 2010
	\author	Nestal Wan
*/

#include "MockFont.hh"

#include "core/Ref.hh"

/**	constructor
*/
MockFont::MockFont( )
{
}

std::string MockFont::BaseName( ) const
{
	return "" ;
}

pdf::Ref MockFont::Write( pdf::IFile *file ) const
{
	return pdf::Ref( ) ;
}

pdf::FontDescriptor* MockFont::Descriptor( )
{
	return 0 ;
}

double MockFont::Width( const std::wstring& text, double size ) const
{
	return 0.0 ;
}

FT_Face MockFont::Face( ) const
{
	return 0 ;
}

