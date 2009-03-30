/***************************************************************************\
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
\***************************************************************************/

/*!
	\file	SymbolInfo.hh
	\brief	definition the SymbolInfo class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/

#ifndef __PDF_SYMBOL_INFO_HEADER_INCLUDED__
#define __PDF_SYMBOL_INFO_HEADER_INCLUDED__

#include <memory>
#include <iosfwd>

namespace pdf {

/*!	\brief	brief description
	
	this class represents
*/
class SymbolInfo
{
public :
	SymbolInfo( ) ;
	~SymbolInfo( ) ;

	static SymbolInfo* Instance( ) ;

	void Backtrace( std::ostream& os,
	                std::size_t limit = 999 ) const ;

private :
	struct Impl ;
	const std::auto_ptr<Impl> m_impl ;
	
	struct BacktraceInfo ;
	friend struct BacktraceInfo ;
} ;

} // end of namespace

#endif
