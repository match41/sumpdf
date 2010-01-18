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

/**	\file	MockFont.hh
    \brief	definition the MockFont class
    \date	Jan 18, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_MOCKFONT_HH_EADER_INCLUDED__
#define __PDF_MOCKFONT_HH_EADER_INCLUDED__

#include "font/BaseFont.hh"

#include "font/FontDescriptor.hh"

///	brief description
/**	The MockFont class represents
*/
class MockFont : public pdf::BaseFont
{
public :
	MockFont( ) ;

	std::string BaseName( ) const ;
	pdf::Ref Write( pdf::IFile *file ) const ;
	pdf::FontDescriptor* Descriptor( ) ;
	double Width( const std::wstring& text, double size ) const ;
	FT_Face Face( ) const ;
} ;

#endif // MOCKFONT_HH_
