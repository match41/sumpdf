/***************************************************************************\
 *   Copyright (C) 2009 by Nestal Wan                                      *
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
 \***************************************************************************/

/**
	\file	CatalogTest.cpp
	\brief	definition the CatalogTest class
	\date	Dec 13, 2009
	\author	Nestal Wan
*/

#include "CatalogTest.hh"

#include "mock/MockFile.hh"

#include <sstream>

CatalogTest::CatalogTest( )
{
}

void CatalogTest::TestRead( )
{
	std::ostringstream ss( "<</PageLayout /SinglePage/PageMode /UseNode"
	                       "/Pages 1 0 R/Type /Catalog/Version /1.4>>" ) ;

}
