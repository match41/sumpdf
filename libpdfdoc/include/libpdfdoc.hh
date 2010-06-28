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
	\file	libpdfdoc.hh
	\brief	definition the libpdfdoc class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __LIB_PDF_DOC_HEADER_INCLUDED__
#define __LIB_PDF_DOC_HEADER_INCLUDED__

#define LIBPDFDOC_API 

#include <string>

/**	\mainpage	libpdfdoc Library Documentation

	libpdfdoc is a library for reading and writing PDF files. It is part of the
	<a href="https://sourceforge.net/apps/trac/libpdfxx/wiki/WikiStart">libpdf++
	</a> project. This section will describe how to write a simplest "Hello
	world" program with libpdfdoc.

	Let's start with the following code:

\code
pdf::Doc *doc = pdf::CreateDoc( ) ;
pdf::Page *p = doc->AppendPage( ) ;
pdf::Font *f = doc->CreateSimpleFont( "Arial" ) ;

pdf::PageContent *c = p->GetContent( ) ;
pdf::Text *t = c->AddText( 12.0, f ) ;
t->AddLine( 100, 100, L"Hello world!" ) ;

doc->Write( "helloworld.pdf" ) ;
delete doc ;
\endcode

	The first line creates a PDF document. The CreateDoc() function will
	create a Doc object, which represents a PDF document. The Doc object
	provides functions to access the document content. It has member functions
	to get the individual pages of the document. The document object has
	ownership over all objects associate with it, including the pages, fonts and
	visual elements. Therefore there is no need to delete these objects.
	
	The second line call pdf::Doc::AppendPage() to insert a new page at the end
	of the document.
	
	\section	links	Links
	
	- <a href="http://sourceforge.net/apps/trac/libpdfxx/wiki/WikiStart">
	libpdf++ Project Homepage</a>
	- <a href="http://gitorious.org/libpdfxx">Gitorious hosted repository</a>
*/

///	Main namespace for libpdfdoc
/**	This namespace contains almost everything in libpdfdoc and pdfedit.
*/
namespace pdf {

class Doc ;
class GraphicsState ;
class Matrix ;
class Page ;
class Path ;
class Text ;
class TextLine ;

LIBPDFDOC_API Doc*  CreateDoc( ) ;

LIBPDFDOC_API const char* Version( ) ;

LIBPDFDOC_API Text* CreateText( const GraphicsState& ts ) ;
LIBPDFDOC_API Path* CreatePath( const GraphicsState& ts ) ;
LIBPDFDOC_API TextLine* CreateTextLine(
	const GraphicsState& 	state,		
	const Matrix&			transform,
	const std::wstring&		text		= std::wstring() ) ;

} // end of namespace

#endif
