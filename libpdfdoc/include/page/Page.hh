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
	\file	Page.hh
	\brief	definition the Page class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __PDF_PAGE_HEADER_INCLUDED__
#define __PDF_PAGE_HEADER_INCLUDED__

#include <string>

namespace pdf {

class Rect ;
class Font ;
class PageContent ;

/**	\defgroup	page	Page and Content Related Classes
	These classes are used to manipulate pages in a PDF document and their
	contents.
*/

///	A page in a PDF document.
/**	\ingroup page
	This class represents a page in a PDF document. It is created by calling
	Doc::AddPage(). It contains function to allow modifying the content of
	the page, e.g. DrawText().
*/
class Page
{
public :
	virtual ~Page( ) ;

	///	get the media box of the page
	/**	According to the PDF specification, the media box is a rectangle
		expressed in default user space units, defining the boundaries of
		the physical medium on which the page is intended to be displayed or
		printed.
	*/
	virtual Rect MediaBox( ) const = 0 ;
	
	virtual Rect CropBox( ) const = 0 ;

	virtual PageContent* GetContent( ) = 0 ;
} ;

} // end of namespace

#endif
