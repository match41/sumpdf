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

#include "Doc.hh"
#include "DocInfo.hh"
#include "libpdfdoc.hh"
#include "font/Font.hh"
#include "graphics/Text.hh"
#include "graphics/TextLine.hh"
#include "graphics/TextState.hh"
#include "page/Page.hh"

#include "util/Backtrace.hh"

#include "util/Util.hh"
#include <boost/bind.hpp>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>

int main( int argc, char **argv )
{
	pdf::Doc *doc = pdf::CreateDoc( ) ;
	if ( argc >= 2 )
	{
		try
		{
			doc->Read( argv[1] ) ;
		}
		catch ( std::exception& e )
		{
			std::cout << e.what() ;
			return -1 ;
		}
	}
	pdf::Page *p = doc->AppendPage( ) ;
	pdf::Font *f = doc->CreateSimpleFont( "Arial" ) ;
	
//	pdf::PageContent *c = p->GetContent( ) ;
	pdf::TextState ts ;
	ts.SetFont( 12.0, f ) ;
	pdf::Text *t = pdf::CreateText( pdf::GraphicsState(ts) ) ;
	t->AddLine( 100, 100, L"Hello world!" ) ;
	std::vector<pdf::Graphics*> gfx( 1, t ) ;
	p->SetContent( gfx ) ;

	doc->Write( "test.pdf" ) ;
	delete doc ;

	return 0 ;
}
