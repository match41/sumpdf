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

#include "core/Array.hh"
#include "file/Function.hh"

#include <map>

namespace pdf {

class DictReader ;
class File ;
class GraphicsState ;
class Ref ;

///	brief description
/**	\internal
	The StateParamDict class represents
*/
class ExtGState : public RefCounter
{
public :
	ExtGState( ) ;
	explicit ExtGState( DictReader& dict ) ;
	
	void Read( DictReader& dict ) ;
	Ref Write( File *file ) const ;

	void Apply( GraphicsState& gs ) const ; 

private :
	enum Field
	{
		// device independent
		line_width, line_cap, line_join, miter_limit, dash_pattern,
		font,
		
		black_generation
	} ;

	std::map<Field, double>		m_doubles ;
	std::map<Field, Array>		m_arrays ;
	std::map<Field, Function*>	m_func ;
} ;

} // end of namespace

#endif // STATEPARAMDICT_HH_
