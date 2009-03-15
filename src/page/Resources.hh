/***************************************************************************
 *   Copyright (C) 2006 by Nestal Wan                                      *
 *   me@nestal.net                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
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
 ***************************************************************************/

/*!
	\file	Resources.hh
	\brief	definition the Resources class
	\date	Sat Mar 22 2008
	\author	Nestal Wan
*/

#ifndef __PDF_RESOURCES_HEADER_INCLUDED__
#define __PDF_RESOURCES_HEADER_INCLUDED__

#include "file/IElement.hh"

#include "core/Dictionary.hh"
#include "core/Name.hh"

#include <vector>

namespace pdf {

class IElementDest ;
class IElementSrc ;
class IFile ;
class SimpleFont ;
class RealImage ;
class XObject ;

/*!	\brief	brief description
	
	this class represents
*/
class Resources : public IElement
{
public :
	Resources( ) ;
	Resources( const Dictionary& dict, IElementSrc *repo ) ;

	Name AddFont( SimpleFont *font ) ;

	void Read( const Dictionary& dict, IElementSrc *repo ) ;
	void Read( const Ref& link, IElementSrc *repo ) ;
	void Write( const Ref& link, IElementDest *repo ) const ;

	ElementList GetChildren( ) const ;
	
private :
	void OnRead( IElementSrc *repo ) ;

	template <typename T>
	void ReadSubDict( const Name& name, IElementSrc *file,
	                  std::map<Name, T*>& output ) ;
	
	template <typename T>
	Dictionary WriteSubDict( const std::map<Name, T*>& input,
	                         IElementDest *repo ) const ;

	XObject* ReadXObj( const Ref& link ) ;

private :
	Dictionary			m_self ;
	Dictionary			m_ext_gstate ;

	typedef std::map<Name, SimpleFont*> FontMap ;
	FontMap				m_fonts ;

	typedef std::map<Name, RealImage*> ImageMap ;
	ImageMap			m_images ;

	std::map<Name, XObject*>	m_xobjs ;

	std::vector<Name>	m_proc_set ;
} ;

} // end of namespace

#endif
