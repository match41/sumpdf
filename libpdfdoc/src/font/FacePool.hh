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

/**	\file	FacePool.hh
    \brief	definition the FacePool class
    \date	Jan 17, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_FACEPOOL_HEADER_INCLUDED__
#define __PDF_FACEPOOL_HEADER_INCLUDED__

#include <ft2build.h>
#include FT_CACHE_H

#include <map>
#include <string>
#include <vector>

namespace pdf {

class IFile ;
class Ref ;

///	brief description
/**	The FontPool class represents
*/
class FacePool
{
public :
	explicit FacePool( FT_Library lib ) ;
	~FacePool( ) ;

	FT_Face GetFace( const Ref& ref, IFile *file ) ;
	
	FT_Face GetFace(
		const std::string& font_name,
		const unsigned char *data,
		std::size_t size ) ;
	
	FT_Glyph GetGlyph( FT_Face face, wchar_t ch ) ;

private :
	static FT_Error RequestFace(
		FTC_FaceID	face_id,
		FT_Library	library,
		FT_Pointer	request_data,
		FT_Face		*face ) ;

	struct FaceID
	{
		std::vector<unsigned char>	data ;
	} ;

	FT_Face LookUpFace( FaceID *face_id ) ;

private :
	FT_Library		m_ft ;
	FTC_Manager		m_mgr ;
	FTC_ImageCache	m_img ;
	FTC_CMapCache	m_cmap ;

	typedef std::map<Ref,	FaceID*> RefFaceMap ;
	RefFaceMap	m_ref_map ; 

	typedef std::map<std::string,	FaceID*> NameFaceMap ;
	NameFaceMap	m_name_map ; 
} ;

} // end of namespace

#endif // FONTPOOL_HH_
