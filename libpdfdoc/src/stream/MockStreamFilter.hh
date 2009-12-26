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

/**
 \file	MockStreamFilter.h
 \brief	definition the MockStreamFilter class
 \date	Dec 22, 2009
 \author	nestal
 */

#ifndef __PDF_MOCKSTREAMFILTER_H_EADER_INCLUDED__
#define __PDF_MOCKSTREAMFILTER_H_EADER_INCLUDED__

#include "StreamFilter.hh"

namespace pdf {

class MockStreamFilter : public StreamFilter
{
public:
	MockStreamFilter( ) ;

	std::size_t Read( unsigned char *data, std::size_t size ) ;
	std::size_t Write( const unsigned char *data, std::size_t size ) ;
	void Flush( ) ;
	void Rewind( ) ;
	std::size_t Length( ) const ;
	Object GetFilterName( ) const ;
	StreamFilter* Clone( ) const ;
	StreamFilter* GetInner( ) ;
} ;

} // end of namespace

#endif // MOCKSTREAMFILTER_H_
