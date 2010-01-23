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

/**	\file	Debug.hh
    \brief	definition the Debug class
    \date	Jan 17, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_DEBUG_HH_EADER_INCLUDED__
#define __PDF_DEBUG_HH_EADER_INCLUDED__

// debug mode only
#ifndef NDEBUG

namespace pdf
{
	/// Called when assertion failure. 
	/**	This function will be called when assertion failure. It will try to
		print out the call stack and abort the program.
	*/
	void AssertFail( const char *text, const char *file, unsigned line ) ;
}

#define PDF_ASSERT( expr )				\
  ((expr)								\
   ? static_cast<void>(0)				\
   : pdf::AssertFail( #expr, __FILE__, __LINE__ ) )

#else
#define PDF_ASSERT( expr )	(static_cast<void>(0))
#endif

#endif // DEBUG_HH_
