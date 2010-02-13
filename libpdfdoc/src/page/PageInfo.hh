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

/**	\file	PageInfo.hh
    \brief	definition the PageInfo class
    \date	Feb 13, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_PAGEINFO_HH_EADER_INCLUDED__
#define __PDF_PAGEINFO_HH_EADER_INCLUDED__

#include "util/Rect.hh"

namespace pdf {

class DictReader ;
class Dictionary ;
class File ;
class PageTree ;
class RealResources ;
class FontDb ;

///	brief description
/**	\internal
	The PageInfo class represents common properties of PageTree and RealPage.
*/
class PageInfo
{
public :
	explicit PageInfo( PageTree *parent ) ;
	explicit PageInfo( FontDb *fontdb ) ;
	~PageInfo( ) ;

	void Read( DictReader& dict ) ;
	void Write( Dictionary& dict, File *file ) const ;

	RealResources* GetResource( ) ;
	const RealResources* GetResource( ) const ;

	PageTree* Parent( ) ;

	Rect MediaBox() const ;
	Rect CropBox() const ;

private :
	PageTree		*m_parent ;
	RealResources	*m_res ;
	
	Rect		m_media_box ;
	Rect		m_crop_box ;

	int	m_rotate ;
} ;

} // end of namespace

#endif // PAGEINFO_HH_
