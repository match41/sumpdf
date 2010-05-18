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

/**	\file	ElementFactory.hh
    \brief	definition the ElementFactory class
    \date	Mar 7, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_DICTREADERIMPL_HH_EADER_INCLUDED__
#define __PDF_DICTREADERIMPL_HH_EADER_INCLUDED__

#include "DictReader.hh"
#include "ElementPool.hh"
#include "File.hh"

#include <limits>

namespace pdf {

template <typename T=DictReader>
class ElementFactory
{
public :
	explicit ElementFactory( DictReader& dict )
	: m_dict( dict )
	{
	}

	template <typename Element, typename Factory>
	Element* Create( Dictionary::iterator i, Factory func )
	{
		ElementPool *pool = m_dict.GetFile()->Pool() ;
		Element *result = 0 ;
		
		// it's good if it's a reference to something already in the pool
		if ( i != m_dict->end() && i->second.Is<Ref>() &&
		     pool->Acquire( i->second, result ) )
			return result ;

		// otherwise we need to create it and maybe add it in the pool
		if ( i != m_dict->end() )
		{
			T temp ;
			bool is_ref = m_dict.SwapAt( i, temp ) ;
			result = func( temp ) ;

			if ( is_ref )
				pool->Add( i->second, result ) ;
		}
	
		return result ;
	}
	
	template <typename Element, typename Factory>
	Element* Create( const Name& name, Factory func, Element *original = 0 )
	{
		Dictionary::iterator i = m_dict->find( name ) ;
		Element *e = Create<Element>( i, func ) ;
		if ( e != 0 )
		{
			m_dict->erase( i ) ;
			if ( original != 0 )
				original->Release( ) ;
		}
		return e ;
	}
	
	template <typename Element, typename Factory, typename OutIt>
	void MassProduce( Factory func, OutIt out,
		std::size_t count = std::numeric_limits<std::size_t>::max() )
	{
		for ( Dictionary::iterator i = m_dict->begin(); i != m_dict->end(); ++i)
			*out++ = std::make_pair( i->first, Create<Element>( i, func ) ) ;
	}

private :
	DictReader m_dict ;
} ;

} // end of namespace

#endif // DICTREADERIMPL_HH_
