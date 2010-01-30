/***************************************************************************
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
 ***************************************************************************/

/*!
	\file	libpdfdoc.hh
	\brief	definition the libpdfdoc class
	\date	Fri Apr 11 2008
	\author	Nestal Wan
*/

#ifndef __LIB_PDF_DOC_HEADER_INCLUDED__
#define __LIB_PDF_DOC_HEADER_INCLUDED__

#define LIBPDFDOC_API 

/*!	\mainpage	libpdfdoc Project Home Page
	\section	intro	Introduction
	libpdfdoc is a library to access
	<a href="http://www.adobe.com/devnet/pdf/pdf_reference.html">PDF (Portable
	Document Format)</a>. It provides classes
	and functions for reading and writing to PDF files.
	
	\section	features	Features
	- read/write PDF files with the same API
	- not much right now
	
	\section	license		License
	libpdfdoc is release under
	<a href="http://www.gnu.org/licenses/gpl-2.1.html">GPL v2</a>.
	In short, it is <a href="http://www.gnu.org/philosophy/free-sw.html">Free
	Software.</a> I am not responsible for any damage it may make. You can
	change it, sell it,	distribute it. As libpdfdoc contains some code borrow
	from GPL project, the license is GPL v2 as a whole. As it is NOT LGPL,
	tt is not legal to use it in proprietory projects.
	
	Note that it is not "GPL v2 or later". The GPL v3 does not apply to this
	library.
	
	\section	get		Getting libpdfdoc
	libpdfdoc can be downloaded in from
	<a href="http://sourceforge.net/project/showfiles.php?group_id=14057">
	sourceforge</a>. The latest snapshot is also available in the
	<a href="http://libpdfxx.git.sourceforge.net/">git</a> repository hosted
	in sourceforge. To access the repository, configure you git clients to
	clone from git://libpdfxx.git.sourceforge.net/gitroot/libpdfxx.
	
	\section	depend	Dependencies
	libpdfdoc depends on the following libraries:
	- <a href="http://www.boost.org">The Boost Library</a>
	- <a href="http://zlib.net">Zlib</a>
	- <a href="http://www.freetype.org">Freetype</a>
	- <a href="http://libpng.sourceforge.net/index.html">libpng</a> (Optional)
	- <a href="http://www.ijg.org/">libjpeg</a> (Optional)
	- <a href= 
	"http://apps.sourceforge.net/mediawiki/cppunit/index.php?title=Main_Page">
	cppunit</a> (Optional)
	
	\section	build	Building libpdfdoc
	libpdfdoc uses the <a href="http://www.cmake.org/">cmake</a> build system
	to produce makefiles for different development platforms. The cmake build
	system will automatically detect the location of the dependency libraries
	and adjust the include and library path of the project. In most unix-like
	environment, the command:

\verbatim
cmake -G 'Unix Makefiles' /path/to/libpdfdoc/
\endverbatim

	should be enough to generate the makefiles to build the project.
	It is also possible to use cmake to generate project files of other build
	environments, e.g. MS Visual Studio or eclipse, depending on your platform.
	Afterwards, just type \c make to build. cmake by default will not show the
	actual command line of the compiler. To see it, type this command:

\verbatim
VERBOSE=1 make
\endverbatim

	This will show the full command line of the compiler. It is useful for
	troubleshooting compiler issues.

	cmake also supports Microsoft Visual Studio. Just use a different makefile
	generator for the version of VS. For example, for VS 2005:
	
\verbatim
cmake -G 'Visual Studio 8 2005' c:\path\to\libpdfdoc\
\endverbatim

	You may have to add <tt>-DCMAKE_PREFIX_PATH=c:\\path\\to\\libraries</tt>
	to the cmake command line to make sure it can find the library
	dependencies. You can also use
	<tt>-DBOOST_ROOT=c:\\path\\to\\boost\\library</tt> to specify
	the path to the boost library.

	cmake recommends separating the source code directory and the build
	directory. Suppose the libpdfdoc source code is located at
	<tt>c:\\code\\libpdfdoc</tt>, and you want to create the VS project files
	at <tt>c:\\build\\libpdfdoc</tt>, you may want to run the following
	commands:
	
\verbatim
C:\build\libpdfdoc>cmake -G "Visual Studio 8 2005" \
                         -DBOOST_ROOT=c:\path\to\boost\library \
                         -DCMAKE_PREFIX_PATH=c:\path\to\other\libraries \
                         -DCPPUNIT_PREFIX=c:\path\to\cppunit \
                         c:\code\libpdfdoc
\endverbatim

	<a href="http://sourceforge.net">
		<img src="http://sourceforge.net/sflogo.php?group_id=14057&amp;type=5"
	         width="210" height="62" border="0" alt="SourceForge Logo" />
	</a>
*/

///	main namespace for libpdfdoc
/**	This namespace contains almost everything in libpdfdoc.
*/
namespace pdf {

class Doc ;
class Page ;

LIBPDFDOC_API Doc*  CreateDoc( ) ;

LIBPDFDOC_API const char* Version( ) ;

} // end of namespace

#endif
