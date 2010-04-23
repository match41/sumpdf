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

/**	\file	BaseEncoding.hh
    \brief	definition the BaseEncoding class
    \date	Apr 24, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_BASEENCODING_HH_EADER_INCLUDED__
#define __PDF_BASEENCODING_HH_EADER_INCLUDED__

// base class headers
#include "util/RefCounter.hh"
#include "font/FontEncoding.hh"

namespace pdf {

///	brief description
/**	\internal
	The BaseEncoding class represents
*/
class BaseEncoding : public RefCounter, public FontEncoding
{
public :
	virtual ~BaseEncoding( ) ;

	virtual wchar_t ToUnicode( unsigned short char_code ) const = 0 ;
	virtual unsigned short FromUnicode( wchar_t unicode ) const = 0 ;
} ;

} // end of namespace

#endif // BASEENCODING_HH_
