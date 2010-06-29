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

/**	\file	PredictFilter.hh
    \brief	definition the PredictFilter class
    \date	Jun 25, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_PREDICTFILTER_HEADER_INCLUDED__
#define __PDF_PREDICTFILTER_HEADER_INCLUDED__

#include "StreamFilter.hh"

#include <memory>
#include <vector>

namespace pdf {

class Object ;

///	brief description
/**	\internal
	The PredictFilter class represents
*/
class PredictFilter : public StreamFilter
{
public :
	explicit PredictFilter( std::auto_ptr<StreamFilter> src, std::size_t col ) ;

	std::size_t Read( unsigned char *data, std::size_t size ) ;
	std::size_t Write( const unsigned char *data, std::size_t size ) ;
	void Flush( ) ;
	void Rewind( ) ;
	std::size_t Length( ) const ;
	Object NameChain( ) const ;
	StreamFilter* Clone( ) const ;
	StreamFilter* GetInner( ) ;
	
	Name RawFormat( ) const ;

private :
	bool FillBuffer( ) ;

private :
	const std::auto_ptr<StreamFilter>	m_src ;

	// data that is suppose to return by Read()
	std::vector<unsigned char>			m_buffer ;
	std::size_t							m_idx ;
} ;

} // end of namespace

#endif // PREDICTFILTER_HH_
