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

/**	\file	FontException.hh
    \brief	definition the FontException class
    \date	Jan 23, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FONTEXCEPTION_HH_EADER_INCLUDED__
#define __PDF_FONTEXCEPTION_HH_EADER_INCLUDED__

#include "util/Exception.hh"

namespace pdf {

///	brief description
/**	The FontException class represents
*/
struct FontException : virtual Exception {} ;

namespace expt
{
	// back-trace information. should be present for all exceptions
	typedef boost::error_info<struct FtErrTag, int>	FTError ;
	
	std::string LookUpFtError( int fterror ) ;
}

} // end of namespace

#endif // FONTEXCEPTION_HH_
