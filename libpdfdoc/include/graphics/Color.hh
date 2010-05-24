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

/**	\file	Colour.hh
    \brief	definition the Colour class
    \date	Feb 17, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_COLOUR_HEADER_INCLUDED__
#define __PDF_COLOUR_HEADER_INCLUDED__

#include <cstddef>
#include <iosfwd>

namespace pdf {

/// Class representing a colour.
/**	\internal
	The Colour class represents
*/
class Color
{
public :
	enum Space
	{
		rgb,
		gray,
		cmyk
	} ;

	typedef const double* iterator ;

public :
	/// default colour is gray black.
	Color( ) ;
	
	/// grayscale
	explicit Color( double gray ) ;
	
	/// RGB colour
	Color( double r, double g, double b ) ;
	
	/// CMYK colour
	Color( double c, double m, double y, double k ) ;
	
	Color( Space cs, double *channel ) ;
	Color( Space cs, unsigned char *channel ) ;
	
	Space ColorSpace( ) const ;

	std::size_t ChannelCount( ) const ;
	static std::size_t ChannelCount( Space sp ) ;

	/// grayscale
	void AssignGray( double gray ) ;
	
	/// RGB colour
	void AssignRGB( double r, double g, double b ) ;
	
	/// CMYK colour
	void AssignCMYK( double c, double m, double y, double k ) ;
	
	//@{
	/// RGB channels
	double Red( ) const ;
	double Green( ) const ;
	double Blue( ) const ;
	//@}
	
	//@{
	/// CMYK channels
	double Cyan( ) const ;
	double Magenta( ) const ;
	double Yellow( ) const ;
	double Black( ) const ;
	//@}
	
	/// gray channel
	double Gray( ) const ;

	bool operator==( const Color& colour ) const ;
	bool operator!=( const Color& colour ) const ;

	iterator begin( ) const ;
	iterator end( ) const ;
	
	unsigned Quad() const ;

private :
	Space	m_cs ;
	double	m_channel[4] ;
} ;

std::ostream& operator<<( std::ostream& os, const Color& t ) ;

} // end of namespace



#endif // COLOUR_HH_
