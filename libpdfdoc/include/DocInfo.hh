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
	\file	DocInfo.hh
	\brief	definition the DocInfo class
	\date	Fri Apr 3 2009
	\author	Nestal Wan
*/

#ifndef __PDF_DOC_INFO_HEADER_INCLUDED__
#define __PDF_DOC_INFO_HEADER_INCLUDED__

#include <string>

namespace pdf {

/*!	\brief	PDF document info. 
	
	\param	producer	if the document was converted to PDF from another
						format, the name of the application (for example,
						libpdfdoc) that converted it to PDF.
	\param	creator		if the document was converted to PDF from another
						format, the name of the application (for example,
						Adobe FrameMaker) that created the original document
						from which it was converted.
*/
class DocInfo
{
protected :
	~DocInfo( ) ;

public :
	virtual const std::string& Creator() const = 0 ;
	virtual const std::string& Producer() const = 0 ;
	
	virtual void SetCreator( const std::string& creator ) = 0 ;
} ;

} // end of namespace

#endif
