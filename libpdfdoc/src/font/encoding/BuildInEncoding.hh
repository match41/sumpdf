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

/**	\file	BuildInEncoding.hh
    \brief	definition the BuildInEncoding class
    \date	May 4, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_BUILDINENCODING_HH_EADER_INCLUDED__
#define __PDF_BUILDINENCODING_HH_EADER_INCLUDED__

#include "BaseEncoding.hh"

namespace pdf {

class Name ;

///	brief description
/**	\internal
	The BuildInEncoding class represents
*/
class BuildInEncoding : public BaseEncoding
{
public :
	explicit BuildInEncoding( const Name& name ) ;

	Object Write( File *file ) const ;
	
private :
	enum Encoding { win, mac_roman, mac_expert, standard } m_enc ;

private :
	static Encoding Parse( const Name& name ) ;
	void Build( const char **map ) ;
} ;

} // end of namespace

#endif // BUILDINENCODING_HH_
