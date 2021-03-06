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

/**	\file	Doc.hh
	\brief	definition the Doc class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __PDF_DOC_HEADER_INCLUDED__
#define __PDF_DOC_HEADER_INCLUDED__

#include <string>

struct FT_FaceRec_ ;

namespace img
{
	class Pixmap ;
}

namespace pdf {

class DocInfo ;
class Font ;
class Image ;
class Page ;

///	PDF document class.
/**	This class represents a PDF document. It is the main object that clients
	need to work with PDF documents. It has functions to read and write
	the PDF document from and to a file, as well as functions to manipulate
	the structure of the document like inserting or removing a page.
	
	
*/
class Doc
{
public :
	virtual ~Doc( ) ;

	/**	\name	File I/O functions
		Functions reading from and writing to files. Call these functions to
		read a PDF document from a file, or write the PDF document to a file.
	*/
	//@{
	///	Read a PDF document from a file
	/**	This function reads a PDF document from a file. It will not read
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
							file. The file cannot be read.
		\sa		Write()
	*/
	virtual void Read( const std::string& filename ) = 0 ;
	
	///	Write the PDF document to a file
	/**	This function writes the PDF document to a file. The file should not be
		the one that is specified in Read(). During Write(), all page content
		of the PDF document will be read and saved in \a filename.
		
		As Write() is a constant member function. It can be called multiple
		times without changing the content of the document.
		
		\param	filename	The name of the file to be written to. If it does
							not exist, it will be created. It must be writable.
		\throw	ParseError	is thrown when there is parser error when reading
							the page contents from the file.
		\sa	 Read()
	*/
	virtual void Write( const std::string& filename ) const = 0 ;
	//@}
	
	/**	\name	Page Accessing Functions
		These functions allow adding and remove pages from the document.
	*/
	//@{
	/// Append a page at the end of the document.
	/** Add a page to the document. This function creates a new page and add
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
	
	/**	\name	Resources Functions
		These functions provide interfaces to accessing the resources of the
		document.
	*/
	//@{
	///	Create a font to be used with the document.
	/**	This is a factory function creates a new font object for use with this
		PDF document. The font object can be used with the TextState objects
		to specify the font of a text object in a page.
		
		\note Only this document can use the font object. Moreover, if the
			  document which created the font object is destroyed, the font
			  object will be also destroyed as well.
		 
		\param	name	the name of the font, e.g. "Arial"
		\return	A pointer to the newly created font object. The pointer pointed
				by it will be invalidated after the document is destroyed. The
				caller does not need to delete it after use. 
		\sa		Font, Page, TextState
	*/
	virtual Font* CreateSimpleFont( FT_FaceRec_ *face ) = 0 ;
	virtual Font* CreateSimpleFont(
		const unsigned char		*data,
		std::size_t 			size ) = 0 ;
	//@}
	
	/// Add an image to the document.
	/**	Call this function to add an image \a img to the document for later use.
		The \a img must be an object created by new. After adding the image to
		the document, the image will be owned by the document, i.e. the caller
		does not need to delete it.
		\param	img		the image to be added. It will be owned by the document
						after adding.
		\return	A newly created Image object. It can be added to a Page of the
				document later.
	*/
	virtual Image* AddImage( img::Pixmap *img ) = 0 ;
	
	virtual const DocInfo* Info( ) const = 0 ;
	virtual DocInfo* Info( ) = 0 ;
	
	// exception objects
} ;

} // end of namespace

#endif
