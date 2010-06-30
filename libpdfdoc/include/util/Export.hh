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
	\file	Export.hh
	\brief	definitions of the export macros
	\date	Wed Jun 30 2010
	\author	Nestal Wan
	
	This file contains some utility functions that is not PDF specific.
*/

#ifndef __PDF_EXPORT_HEADER_INCLUDED__
#define __PDF_EXPORT_HEADER_INCLUDED__

#ifdef PDFDOC_DLL
	#ifdef pdfdoc_EXPORTS
		#define LIBPDFDOC_API __declspec(dllexport) 
	#else
		#define LIBPDFDOC_API __declspec(dllimport) 
	#endif
#else
	#define LIBPDFDOC_API
#endif

#endif
