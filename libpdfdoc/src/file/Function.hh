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

/**	\file	Function.hh
    \brief	definition the Function class
    \date	Mar 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FUNCTION_HH_EADER_INCLUDED__
#define __PDF_FUNCTION_HH_EADER_INCLUDED__

#include "util/RefCounter.hh"

#include <boost/shared_ptr.hpp>

namespace pdf {

class DictReader ;
class File ;
class Ref ;
class Object ;
class Stream ;

///	brief description
/**	\internal
	The Function class represents
*/
class Function : public RefCounter
{
public :
	Function( ) ;

	void Read( Object& obj, File *file ) ;
	Ref Write( File *file ) ;

private :
	void ReadCommon( DictReader& dict ) ;
	void ReadType0( DictReader& dict, Stream& data ) ;

private :
	struct Impl ;
	const boost::shared_ptr<Impl>	m_impl ;
} ;

} // end of namespace

#endif // FUNCTION_HH_
