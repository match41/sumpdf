/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
	\file	Doc.hh
	\brief	definition the Doc class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __PDF_DOC_HEADER_INCLUDED__
#define __PDF_DOC_HEADER_INCLUDED__

#include <string>

namespace pdf {

class Page ;
class Font ;

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
        
        \param  filename    The name of file to be read from. It must be
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
	*/
	virtual void Write( const std::string& filename ) const = 0 ;

    /*! \brief  Add a page to the document.
       
        This function creates a new page and add it to the document. The
        page will be appended at the end of the document. The pointer to
        the newly added page will be returned and the caller can put
        content to the new page.
        \return The newly created page.
    */
	virtual Page* AddPage( ) = 0 ;
	
	virtual std::size_t PageCount( ) const = 0 ;
	virtual Page* GetPage( std::size_t index ) = 0 ;
	
	/*!	\brief	Create a font to be used with the document.
	
		This function creates a new font object that only this document
		can use.
	*/
	virtual Font* CreateSimpleFont( const std::string& name ) = 0 ;
} ;

} // end of namespace

#endif
