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

#include "Types.hh"

#include <memory>
#include <string>
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
	
	std::vector<uchar> CreateSubset(
		const long		*glyphs,
		std::size_t 	size ) const ;

private :
	void LoadTableInfo( ) ;
	void LoadLocation( ) ;
	
	u32 WriteSubset(
		std::streambuf	*str,
		const long		*glyphs,
		std::size_t 	size ) const ;
	
	struct Table ;

	void GenerateTable(
		const long		*glyphs,
		std::size_t 	size,
		std::streambuf	*glyf,
		std::streambuf	*loca ) const ;

	u32 WriteSubsetTables(
		const std::string&	glyf,
		const std::string&	loca,
		std::streambuf		*dest ) const ;
	u32 WriteTableDirectory(
		const std::string&	glyf,
		const std::string&	loca,
		std::streambuf		*dest ) const ;

	Table MakeTable( u32 tag, u32 offset, const std::string& data ) const ;
	Table MakeTable( u32 tag, u32 offset, const uchar *data,
		std::size_t size ) const ;

	/// wrapper for FT_Load_Sfnt_Table()
	std::vector<unsigned char> ReadTable( const Table& tab ) const ;
	Table FindTable( u32 tag ) const ;

	void WriteTableDirEntry( WriteStream& s, const Table& tab ) const ;
	void CopyTable( std::streambuf *s, const Table& tab ) const ;
	void WriteTable(
		std::streambuf	*s,
		const uchar		*data,
		std::size_t		size ) const ;
	
	void CopyGlyph(
		unsigned 		glyph,
		std::streambuf	*glyf,
		std::streambuf 	*loca,
		const uchar		*src,
		std::size_t		size ) const ;
	void WriteGlyphLocation( std::streambuf *loca, u32 value ) const ;
	void WriteGlyphLocation( std::streambuf *loca, std::streambuf *glyf ) const ;

private :
	struct Impl ;
	const std::auto_ptr<Impl> m_impl ;
} ;

} // end of namespace

#endif // SFNT_HH_
