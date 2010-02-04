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

/**	\file	NameTree.hh
    \brief	definition the NameTree class
    \date	Feb 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_NAMETREE_HH_EADER_INCLUDED__
#define __PDF_NAMETREE_HH_EADER_INCLUDED__

#include <map>
#include <string>
#include <vector>

namespace pdf {

class Dictionary ;
class File ;
class Object ;
class Ref ;

///	brief description
/**	\internal
	The NameTree class represents
*/
class NameTree
{
public :
	explicit NameTree( Dictionary& self, File *file ) ;
	~NameTree( ) ;
	
	Ref Write( File *file ) const ;

	const Object& LookUp( const std::string& key ) const ;

private :
	typedef std::map<std::string, Object>	NameMap ;

	std::vector<NameTree*>	m_kids ;
	NameMap					m_names ;
} ;

} // end of namespace

#endif // NAMETREE_HH_
