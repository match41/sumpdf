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

/**	\file	ReferenceCountable.hh
    \brief	definition the ReferenceCountable class
    \date	Oct 12, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_REFERENCECOUNTABLE_HH_EADER_INCLUDED__
#define __PDF_REFERENCECOUNTABLE_HH_EADER_INCLUDED__

#include <cstddef>

namespace pdf {

///	brief description
/**	\internal
	The ReferenceCountable class represents
*/
class ReferenceCountable
{
public :
	virtual ~ReferenceCountable( ) ;
	
	virtual void AddRef( ) = 0 ;
	virtual bool Release( ) = 0 ;
	
	virtual std::size_t UseCount( ) const = 0 ;
} ;

} // end of namespace

#endif // REFERENCECOUNTABLE_HH_
