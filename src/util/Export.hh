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
	\file	Export.hh
	\brief	definition the Export class
	\date	Thu Apr 3 2008
	\author	Nestal Wan
*/

#ifndef __PDF_EXPORT_HEADER_INCLUDED__
#define __PDF_EXPORT_HEADER_INCLUDED__


#ifdef DLL_EXPORTS
    #ifdef _MSC_VER
    	#define LIBPDFDOC_API __declspec( dllexport )
    #else
    	#define LIBPDFDOC_API __attribute__ ((visibility("default")))
    #endif
#else
    #define LIBPDFDOC_API 
#endif

#endif
