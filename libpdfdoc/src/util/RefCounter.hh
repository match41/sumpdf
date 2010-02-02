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
	\file	RefCounter.hh
	\brief	definition the RefCountObj class
	\date	Dec 11, 2009
	\author	nestal
*/

#ifndef __PDF_REFCOUNTOBJ_HEADER_INCLUDED__
#define __PDF_REFCOUNTOBJ_HEADER_INCLUDED__

#include <cstddef>

namespace pdf {

///	\internal	Reference counting helper class
class RefCounter
{
protected :
	RefCounter( ) ;

public :
	virtual void AddRef( ) ;
	virtual bool Release( ) ;
	
	std::size_t UseCount( ) const ;

private :
	std::size_t	m_count ;
} ;

} // end of namespace

#endif // REFCOUNTOBJ_H_
