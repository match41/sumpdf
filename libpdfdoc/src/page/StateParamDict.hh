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

/**	\file	StateParamDict.hh
    \brief	definition the StateParamDict class
    \date	Feb 28, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_STATEPARAMDICT_HH_EADER_INCLUDED__
#define __PDF_STATEPARAMDICT_HH_EADER_INCLUDED__

#include "util/RefCounter.hh"

#include "core/Name.hh"
#include "core/Object.hh"

#include <map>

namespace pdf {

class DictReader ;
class File ;
class Ref ;

///	brief description
/**	\internal
	The StateParamDict class represents
*/
class StateParamDict : public RefCounter
{
public :
	StateParamDict( ) ;
	
	void Read( DictReader& dict ) ;
	Ref Write( File *file ) const ;

private :
	std::map<Name, Object>	m_param ;
} ;

} // end of namespace

#endif // STATEPARAMDICT_HH_
