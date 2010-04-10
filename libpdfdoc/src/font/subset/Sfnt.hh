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

/**	\file	Sfnt.hh
    \brief	definition the Sfnt class
    \date	Apr 9, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_SFNT_HH_EADER_INCLUDED__
#define __PDF_SFNT_HH_EADER_INCLUDED__

#include <memory>
#include <string>
#include <wchar.h>
#include <vector>
#include <iosfwd>

struct FT_FaceRec_ ;

namespace pdf {

class WriteStream ;

///	brief description
/**	\internal
	The Sfnt class represents
*/
class Sfnt
{
public :
	explicit Sfnt( FT_FaceRec_ *face ) ;
	~Sfnt( ) ;
	
	void AddGlyph( wchar_t unicode ) ;
	void Write( std::streambuf *str ) const ;

private :
	void LoadTableInfo( ) ;
	void LoadLocation( ) ;

	struct Table ;

	/// wrapper for FT_Load_Sfnt_Table()
	std::vector<unsigned char> ReadTable( unsigned long tag ) const ;
	Table FindTable( unsigned long tag ) const ;

	void WriteTableDirEntry( WriteStream& s, const Table& tab ) const ;

private :
	struct Impl ;
	std::auto_ptr<Impl>	m_impl ;
} ;

} // end of namespace

#endif // SFNT_HH_
