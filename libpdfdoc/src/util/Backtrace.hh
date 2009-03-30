/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
\***************************************************************************/

/*!
	\file	Backtrace.hh
	\brief	definition the Backtrace class
	\date	Mon Mar 30 2009
	\author	Nestal Wan
*/

#ifndef __PDF_BACK_TRACE_HEADER_INCLUDED__
#define __PDF_BACK_TRACE_HEADER_INCLUDED__

#include <iosfwd>

namespace pdf {

/*!	\brief	a shortcut to print out backtrace information
	
	The sole reason for this class to exists is to provide the
	operator<< overload to allow:
	
\code
std::cout << Backtrace() << std::endl ;
\endcode
	
	\sa SymbolInfo
*/
struct Backtrace {} ;

} // end of namespace

namespace std
{
	ostream& operator<<( ostream& os, const pdf::Backtrace& ) ;
}

#endif
