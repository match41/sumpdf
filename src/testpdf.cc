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

#include "Doc.hh"
#include "Page.hh"
#include "Font.hh"
#include "libpdfdoc.hh"

#include <sstream>
#include <iostream>
#include <cstdlib>

int main( int argc, char **argv )
{
	pdf::Doc *doc = pdf::CreateDoc( ) ;

	if ( argc >= 2 )
		doc->Read( argv[1] ) ;
	
	pdf::Page *p = doc->AddPage( ) ;
	pdf::Font *f = doc->CreateSimpleFont( "Helvetica" ) ;
	p->DrawText( 100, 100, f, "Hello world!" ) ;

	doc->Write( "test.pdf" ) ;

	return 0 ;
}
