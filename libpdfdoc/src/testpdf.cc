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
#include "page/Page.hh"
#include "page/PageContent.hh"

#include "util/Exception.hh"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>

int main( int argc, char **argv )
{
	pdf::Doc *doc = pdf::CreateDoc( ) ;
	if ( argc >= 2 )
		doc->Read( argv[1] ) ;
	
/*	pdf::Page *p = doc->AppendPage( ) ;
	pdf::Font *f = doc->CreateSimpleFont( "Arial" ) ;
	
	pdf::TextState ts ;
	ts.SetFont( 12.0, f ) ;
	
	pdf::PageContent *c = p->GetContent( ) ;
	pdf::Text *t = c->AddText( ts ) ;
	
	pdf::TextLine line1( pdf::Matrix(1,0,0,1, 100, 100), ts ) ;
	line1.AppendText( L"Hello world!" ) ;
	t->AddLine( line1 ) ;

	pdf::TextLine line2( pdf::Matrix(1,0,0,1, 100, 200), ts ) ;
	line2.AppendText( L"This is the second line!" ) ;
	t->AddLine( line2 ) ;

	pdf::DocInfo *info = doc->Info() ;
	info->SetCreator( "Haha" ) ;
*/
	pdf::Page *p = doc->GetPage( 0 ) ;
	pdf::PageContent *c = p->GetContent( ) ;

	doc->Write( "test.pdf" ) ;
	delete doc ;

	return 0 ;
}
