/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan									   *
 *   me@nestal.net														   *
 *																		   *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; version 2.                         	   *
 *																		   *
 *   This program is distributed in the hope that it will be useful,	   *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of		   *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		   *
 *   GNU General Public License for more details.						   *
 *																		   *
 *   You should have received a copy of the GNU General Public License	   *
 *   along with this program; if not, write to the						   *
 *   Free Software Foundation, Inc.,									   *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			   *
 ***************************************************************************/

/*!
	\file	Doc.hh
	\brief	definition the Doc class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __PDF_DOC_HEADER_INCLUDED__
#define __PDF_DOC_HEADER_INCLUDED__

#include <string>

struct FT_FaceRec_ ;

namespace pdf {

class DocInfo ;
class Font ;
class Image ;
class Page ;

/*!	\brief	Base class for documents.
	
	This class represents a PDF document. It is an abstract class that
	all functions are pure virtual. It has functions to read and write
	the PDF document from and to a file, as well as functions to manipulate
	the structure of the document like inserting or removing a page.
*/
class Doc
{
public :
	virtual ~Doc( ) ;

	/*!	\name	File I/O functions
		File input/output functions
	*/
	//@{
	/*! \brief  Read a PDF document from a file
	
		This function reads a PDF document from a file. It will not read
		all data from the file. It will only read the trailer and cross
		reference table to know the structure of the PDF document. Page
		contents will be read on-demand.
		
		The file will be opened throughout the life time of the
		document object. It will be read for document data when
		it is needed. The file will only be closed when the document
		object is destroyed.
		
		The file must be a regular file that supports seeking. Since libpdfdoc
		will read and seek the file when page contents are read on-demand, it
		does not support reading PDF files from a pipe or socket.
		
		\param  filename	The name of file to be read from. It must be
							readable, but not nesscarrily writable.
		\throw	ParseError	is thrown when there is parser error in the PDF
							file.
		\sa		Write()
	*/
	virtual void Read( const std::string& filename ) = 0 ;
	
	/*!	\brief	Write the PDF document to a file
	
		This function writes the PDF document to a file. The file should not be
		the one that is specified in Read(). During Write(), all page content
		of the PDF document will be read and saved in \a filename .
		
		\param	filename	The name of the file to be written to. If it does
							not exist, it will be created. It must be writable.
		\throw	ParseError	is thrown when there is parser error when reading
							the page contents from the file.
		\sa	 Read()
	*/
	virtual void Write( const std::string& filename ) const = 0 ;
	//@}
	
	/*!	\name	Page Accessing Functions
		These functions allow adding and remove pages from the document.
	*/
	//@{
	/*! Add a page to the document. This function creates a new page and add
		it to the document. The page will be appended at the end of the
		document. The pointer to the newly added page will be returned and
		the caller can put content to the new page.
		\return A pointer to the newly created page. This pointer will be
				invalidated after the document is destroyed.
	*/
	virtual Page* AppendPage( ) = 0 ;

	/*! Add a page to the document. It will add a page to the specified
		index.
		\param	index	the index of the old page. If it is too large (i.e.
						larger than the number of pages), the page will
						be added at the end of the document.
		\return A pointer to the newly created page. This pointer will be
				invalidated after the document is destroyed.
	*/
	virtual Page* AddPage( std::size_t index ) = 0 ;
	
	/*! Get the number of pages in the document.
		\return number of page in the document
	*/
	virtual std::size_t PageCount( ) const = 0 ;
	
	/*!	Get the page by its index.
		\param	index	index to the page.
		\return	A pointer to the page, or null if the index does not
				refer to a page. The pointer pointed by it will be
				invalidated after the document is destroyed. 
	*/
	virtual Page* GetPage( std::size_t index ) = 0 ;
	//@}
	
	/*!	\name	Resources Functions
		These functions provide interfaces to accessing the resources of the
		document.
	*/
	//@{
	/*!	\brief	Create a font to be used with the document.
	
		This function creates a new font object that only this document
		can use.
		\param	name	the name of the font, e.g. "Arial"
		\return	A pointer to the newly created font object. The pointer pointed
				by it will be invalidated after the document is destroyed. The
				caller does not need to delete it after use. 
	*/
	virtual Font* CreateSimpleFont( FT_FaceRec_ *face ) = 0 ;
	virtual Font* CreateSimpleFont(
		const unsigned char		*data,
		std::size_t 			size ) = 0 ;
	//@}
	
	virtual Image* LoadImage( const std::string& filename ) = 0 ;
	
	virtual const DocInfo* Info( ) const = 0 ;
	virtual DocInfo* Info( ) = 0 ;
	
	// exception objects
} ;

} // end of namespace

#endif
