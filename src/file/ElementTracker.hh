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
	\file	ElementTracker.hh
	\brief	definition the ElementTracker class
	\date	Sat Apr 19 2008
	\author	Nestal Wan
*/

#ifndef __PDF_ELEMENT_TRACKER_HEADER_INCLUDED__
#define __PDF_ELEMENT_TRACKER_HEADER_INCLUDED__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/tag.hpp>

#include <memory>

namespace pdf {

class IElement ;
class ElementList ;

/*!	\brief	brief description
	
	this element tracker is used to track the dependencies between elements.
*/
class ElementTracker
{
public :
	ElementTracker( ) ;
	~ElementTracker( ) ;	//!< for std::auto_ptr
	
	template <typename InputIt>
	ElementTracker( InputIt first, InputIt last )
		: m_con( first, last )
	{
	}
	
	void Add( IElement *e ) ;
	void Traverse( IElement *e ) ;
	
	void Test( IElement& e ) ;
	
	void Get( ElementList& list ) const ;
	
private :
	struct SeqTag {} ;
	struct MapTag {} ;

	typedef boost::multi_index_container<IElement*,
		boost::multi_index::indexed_by<
			boost::multi_index::sequenced<boost::multi_index::tag<SeqTag> >,
			boost::multi_index::hashed_unique<boost::multi_index::tag<MapTag>,
				boost::multi_index::identity<IElement*> >
		> > Container ;

	typedef Container::index<SeqTag>::type	Seq ;
	typedef Container::index<MapTag>::type	Map ;

	Container	m_con ;
} ;

} // end of namespace

#endif
