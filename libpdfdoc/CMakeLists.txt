#***************************************************************************#
 #   Copyright (C) 2002 by Nestal Wan                                      *
 #   me@nestal.net                                                         *
 #                                                                         *
 #   This program is free software; you can redistribute it and/or modify  *
 #   it under the terms of the GNU General Public License as published by  *
 #   the Free Software Foundation; either version 2 of the License, or     *
 #   (at your option) any later version.                                   *
 #                                                                         *
 #   This program is distributed in the hope that it will be useful,       *
 #   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 #   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 #   GNU General Public License for more details.                          *
 #                                                                         *
 #   You should have received a copy of the GNU General Public License     *
 #   along with this program; if not, write to the                         *
 #   Free Software Foundation, Inc.,                                       *
 #   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
#***************************************************************************#

PROJECT( libpdfdoc )

cmake_minimum_required(VERSION 2.6)

###############################################################################
# required packages
###############################################################################
set( Boost_USE_MULTITHREAD ON )
find_package( Boost REQUIRED )
find_package( ZLIB REQUIRED )

###############################################################################
# optional packages
###############################################################################
find_package( Freetype )
find_package( PNG )
find_package( JPEG )

###############################################################################
# source subdirectories
###############################################################################
set( PAGE_DIR		src/page/ )
set( FILE_DIR		src/file/ )
set( CORE_DIR		src/core/ )
set( STRM_DIR		src/stream/ )
set( IMG_DIR		src/image/ )
set( UTIL_DIR		src/util/ )
set( FONT_DIR		src/font/ )
set( FT_DIR			${FONT_DIR}/ftwrap )

###############################################################################
# headers
###############################################################################
set( PAGE_HDRS		${PAGE_DIR}/RealPage.hh
					${PAGE_DIR}/Resources.hh
					${PAGE_DIR}/PageNode.hh
					${PAGE_DIR}/PageTree.hh
					${PAGE_DIR}/XObject.hh
					${PAGE_DIR}/RealImage.hh
					${PAGE_DIR}/PaintOp.hh
					${PAGE_DIR}/TextOps.hh
					${PAGE_DIR}/PathSegment.hh )

set( FILE_HDRS		${FILE_DIR}/File.hh
					${FILE_DIR}/IFile.hh
					${FILE_DIR}/Catalog.hh
					${FILE_DIR}/StreamElement.hh
					${FILE_DIR}/IElement.hh
					${FILE_DIR}/RawElement.hh
					${FILE_DIR}/ElementReader.hh
            		${FILE_DIR}/ElementDest.hh
            		${FILE_DIR}/IElementDest.hh
             		${FILE_DIR}/ElementList.hh
            		${FILE_DIR}/ElementTracker.hh )

set( CORE_HDRS  	${CORE_DIR}/Object.hh
					${CORE_DIR}/Ref.hh
					${CORE_DIR}/Array.hh
					${CORE_DIR}/Dictionary.hh
					${CORE_DIR}/Token.hh
					${CORE_DIR}/TokenSrc.hh
					${CORE_DIR}/String.hh
					${CORE_DIR}/Name.hh
					${CORE_DIR}/TraverseObject.hh )
					
set( STRM_HDRS		${STRM_DIR}/Stream.hh
					
					# stream filters
					${STRM_DIR}/StreamFilter.hh
					${STRM_DIR}/RawFilter.hh
					${STRM_DIR}/DeflateFilter.hh
					${STRM_DIR}/BufferedFilter.hh
					${STRM_DIR}/StreamBufAdaptor.hh )

set( UTIL_HDRS  	${UTIL_DIR}/Rect.hh
					${UTIL_DIR}/Export.hh
					${UTIL_DIR}/Util.hh
					${UTIL_DIR}/Exception.hh
					${UTIL_DIR}/SymbolInfo.hh
					${UTIL_DIR}/Backtrace.hh )
					
set( IMG_HDRS  		${IMG_DIR}/Image.hh
					${IMG_DIR}/PngImage.hh
					${IMG_DIR}/JpegImage.hh )

set( FONT_HDRS		${FONT_DIR}/TrueTypeFile.hh
					${FONT_DIR}/TrueTypeTable.hh
					${FONT_DIR}/Types.hh

					${FONT_DIR}/BaseFont.hh 
					${FONT_DIR}/SimpleFont.hh
					${FONT_DIR}/StandardFont.hh )

set( FT_HDRS		${FT_DIR}/Face.hh
					${FT_DIR}/Glyph.hh
					${FT_DIR}/Library.hh
					${FT_DIR}/Exception.hh
					${FT_DIR}/ErrorTable.hh )

set( INCLUDE_HDRS	include/Doc.hh
					include/Font.hh
					include/libpdfdoc.hh
					include/Page.hh )

set( BASE_HDRS		src/RealDoc.hh )

###############################################################################
# source in subdirs
###############################################################################
set( PAGE_SRCS  	${PAGE_DIR}/RealPage.cc
					${PAGE_DIR}/Resources.cc
					${PAGE_DIR}/Page.cc
					${PAGE_DIR}/Font.cc
					${PAGE_DIR}/PageNode.cc
					${PAGE_DIR}/PageTree.cc
					${PAGE_DIR}/XObject.cc
					${PAGE_DIR}/RealImage.cc
					${PAGE_DIR}/PaintOp.cc
					${PAGE_DIR}/PathSegment.cc )

set( FILE_SRCS		${FILE_DIR}/File.cc
					${FILE_DIR}/IFile.cc
					${FILE_DIR}/Catalog.cc
					${FILE_DIR}/StreamElement.cc
					${FILE_DIR}/IElement.cc
					${FILE_DIR}/RawElement.cc
					${FILE_DIR}/ElementReader.cc
					${FILE_DIR}/ElementDest.cc
					${FILE_DIR}/IElementDest.cc
					${FILE_DIR}/ElementList.cc
					${FILE_DIR}/ElementTracker.cc )

set( CORE_SRCS  	${CORE_DIR}/Object.cc
					${CORE_DIR}/Ref.cc
					${CORE_DIR}/Array.cc
					${CORE_DIR}/Dictionary.cc
					${CORE_DIR}/Token.cc
					${CORE_DIR}/TokenSrc.cc
					${CORE_DIR}/String.cc
					${CORE_DIR}/Name.cc )

set( STRM_SRCS		${STRM_DIR}/Stream.cc
					
					# stream filters
					${STRM_DIR}/StreamFilter.cc
					${STRM_DIR}/RawFilter.cc
					${STRM_DIR}/DeflateFilter.cc
					${STRM_DIR}/BufferedFilter.cc
					${STRM_DIR}/StreamBufAdaptor.cc )

set( UTIL_SRCS  	${UTIL_DIR}/Rect.cc
					${UTIL_DIR}/Util.cc
					${UTIL_DIR}/Exception.cc
					${UTIL_DIR}/Backtrace.cc )

set( IMG_SRCS  		${IMG_DIR}/Image.cc )

set( FONT_SRCS		${FONT_DIR}/TrueTypeFile.cc
					${FONT_DIR}/TrueTypeTable.cc
					${FONT_DIR}/BaseFont.cc
					${FONT_DIR}/StandardFont.cc )

set( BASE_SRCS		src/Doc.cc
					src/RealDoc.cc
					src/libpdfdoc.cc )

###############################################################################
# unix specific sources
###############################################################################
if ( UNIX )
    set( UTIL_SRCS  ${UTIL_SRCS}
					${UTIL_DIR}/SymbolInfo.cc )
endif ( UNIX )

###############################################################################
# win32 specific sources
###############################################################################
if ( WIN32 )
    set( UTIL_SRCS  ${UTIL_SRCS}
					${UTIL_DIR}/SymbolInfo32.cpp )
endif ( WIN32 )

###############################################################################
# optional sources
###############################################################################

# freetype
if ( FREETYPE_FOUND )
    set( FONT_SRCS  ${FONT_SRCS}
                    ${FONT_DIR}/SimpleFont.cc )
    set( FONT_HDRS  ${FONT_HDRS}
                    ${FONT_DIR}/SimpleFont.hh )
    set( FT_SRCS	${FT_DIR}/Face.cc
					${FT_DIR}/Glyph.cc
					${FT_DIR}/Library.cc
					${FT_DIR}/Exception.cc
					${FT_DIR}/ErrorTable.cc )
    set( FT_HDRS	${FT_DIR}/Face.hh
					${FT_DIR}/Glyph.hh
					${FT_DIR}/Library.hh
					${FT_DIR}/Exception.hh
					${FT_DIR}/ErrorTable.hh )
    set( OPT_LIBS   ${OPT_LIBS}
                    ${FREETYPE_LIBRARIES} )
    set( OPT_INCS   ${OPT_INCS}
                    ${FREETYPE_INCLUDE_DIRS} )
    add_definitions( -DHAVE_FREETYPE )
endif ( FREETYPE_FOUND )

# libpng
if ( PNG_FOUND )
    set( IMG_SRCS   ${IMG_SRCS}
                    ${IMG_DIR}/PngImage.cc )
    set( IMG_HDRS   ${IMG_HDRS}
                    ${IMG_DIR}/PngImage.hh )
    set( OPT_LIBS   ${OPT_LIBS}
                    ${PNG_LIBRARIES} )
    set( OPT_INCS   ${OPT_INCS}
                    ${PNG_INCLUDE_DIR} )
	add_definitions( -DHAVE_PNG )
endif ( PNG_FOUND )

# libjpeg
if ( JPEG_FOUND )
    set( IMG_SRCS   ${IMG_SRCS}
                    ${IMG_DIR}/JpegImage.cc )
    set( IMG_HDRS   ${IMG_HDRS}
                    ${IMG_DIR}/JpegImage.hh )
    set( OPT_LIBS   ${OPT_LIBS}
                    ${JPEG_LIBRARIES} )
    set( OPT_INCS   ${OPT_INCS}
                    ${JPEG_INCLUDE_DIR} )
	add_definitions( -DHAVE_JPEG )
endif ( JPEG_FOUND )

###############################################################################
# all headers
###############################################################################
set( HDRS	${BASE_HDRS}
			${PAGE_HDRS} ${FILE_HDRS} ${CORE_HDRS} ${IMG_HDRS}
			${FONT_HDRS} ${FT_HDRS} ${STRM_HDRS} ${INCLUDE_HDRS} )

###############################################################################
# all sources
###############################################################################
set( SRCS	${BASE_SRCS}
			${PAGE_SRCS} ${FILE_SRCS} ${CORE_SRCS} ${IMG_SRCS} ${UTIL_SRCS}
			${FONT_SRCS} ${FT_SRCS} ${STRM_SRCS} )

###############################################################################
# libpdfdoc library
###############################################################################
#add_library( pdfdoc SHARED ${SRCS} )
add_library( pdfdoc_static STATIC ${SRCS} ${HDRS} )

###############################################################################
# source groups
###############################################################################

SOURCE_GROUP( "page"		FILES ${PAGE_SRCS} ${PAGE_HDRS} )
SOURCE_GROUP( "file"		FILES ${FILE_SRCS} ${FILE_HDRS} )
SOURCE_GROUP( "core"		FILES ${CORE_SRCS} ${CORE_HDRS} )
SOURCE_GROUP( "util"		FILES ${UTIL_SRCS} ${UTIL_HDRS} )
SOURCE_GROUP( "font"		FILES ${FONT_SRCS} ${FONT_HDRS} )
SOURCE_GROUP( "font\\ftwrap"	FILES ${FT_SRCS} ${FT_HDRS} )
SOURCE_GROUP( "image"		FILES ${IMG_SRCS} ${IMG_HDRS} )
SOURCE_GROUP( "stream"		FILES ${STRM_SRCS} ${STRM_HDRS} )
SOURCE_GROUP( "base"		FILES ${BASE_SRCS} ${BASE_HDRS} )
SOURCE_GROUP( "include"		FILES ${INCLUDE_HDRS} )

###############################################################################
# unix specific libraries
###############################################################################
if ( UNIX )
	find_library( DL_LIBRARY 	NAMES dl	PATH /usr/lib /usr/lib64 )
	find_library( BFD_LIBRARY	NAMES bfd	PATH /usr/lib /usr/lib64 )
    set( OPT_LIBS   ${OPT_LIBS}	${DL_LIBRARY} ${BFD_LIBRARY} )
endif ( UNIX )

###############################################################################
# win32 specific libraries
###############################################################################
if ( WIN32 )
    set( OPT_LIBS   ${OPT_LIBS}
                    Dbghelp )
endif ( WIN32 )

###############################################################################
# all libraries
###############################################################################
target_link_libraries( pdfdoc_static
                       ${ZLIB_LIBRARIES}
                       ${OPT_LIBS} )

###############################################################################
# compiler settings
###############################################################################
add_definitions(-DDLL_EXPORTS)
add_definitions(-DTEST_DATA_DIR="${libpdfdoc_SOURCE_DIR}/test/testdata/")
add_definitions(-DSRC_DIR="${libpdfdoc_SOURCE_DIR}/src/")

# MSVC has stupid warnings on STD
if ( MSVC )
	add_definitions( -D_SCL_SECURE_NO_WARNINGS )
	add_definitions( /wd4345 )
endif ( MSVC )

###############################################################################
# finding cppunit
###############################################################################
find_path( CPPUNIT_INCLUDE_DIR cppunit/TestFixture.h /usr/include
                                                     /usr/local/include
                                                     ${CPPUNIT_PREFIX}/include )
find_library( CPPUNIT_LIBRARY_DEBUG NAMES cppunit cppunit_dll
              PATHS /usr/lib
                    /usr/lib64
                    /usr/local/lib
                    /usr/local/lib64
                    ${CPPUNIT_PREFIX}/lib
              PATH_SUFFIXES debug )

find_library( CPPUNIT_LIBRARY_RELEASE NAMES cppunit cppunit_dll
              PATHS /usr/lib
                    /usr/lib64
                    /usr/local/lib
                    /usr/local/lib64
                    ${CPPUNIT_PREFIX}/lib
              PATH_SUFFIXES release )

set( CPPUNIT_LIBRARY debug     ${CPPUNIT_LIBRARY_DEBUG}
                     optimized ${CPPUNIT_LIBRARY_RELEASE} )

if ( CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY )
	message( STATUS "found cppunit" )
	set( CPPUNIT_FOUND TRUE )
	set( OPT_INCS	${OPT_INCS}
	                ${CPPUNIT_INCLUDE_DIR} )
endif ( CPPUNIT_INCLUDE_DIR AND CPPUNIT_LIBRARY )

###############################################################################
# include directories
###############################################################################
include_directories( ${libpdfdoc_SOURCE_DIR}/src
                     ${libpdfdoc_SOURCE_DIR}/include
                     ${libpdfdoc_SOURCE_DIR}/test
                     ${ZLIB_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     ${OPT_INCS} )

INSTALL( TARGETS pdfdoc_static
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib )

INSTALL( FILES ${INCLUDE_HDRS} DESTINATION include )

###############################################################################
# test driver
###############################################################################
add_executable( testpdf src/testpdf.cc )
target_link_libraries( testpdf pdfdoc_static )

###############################################################################
# unit test sources
###############################################################################
if ( CPPUNIT_FOUND )

set( UT_SRCS	test/core/TokenTest.cc
				test/core/TokenSrcTest.cc
				test/core/NameTest.cc
				test/core/RefTest.cc
				test/core/ObjectTest.cc
				test/core/StringTest.cc
				test/core/DictionaryTest.cc
				test/core/ArrayTest.cc
				test/stream/StreamTest.cc
				test/stream/RawFilterTest.cc
				test/stream/DeflateFilterTest.cc
				test/stream/StreamBufAdaptorTest.cc
				
				test/page/PageTest.cc
				test/page/PageNodeTest.cc
				test/page/ResourcesTest.cc
				test/page/XObjectTest.cc
				
				test/file/FileTest.cc
				test/file/ElementReaderTest.cc
				test/file/ElementTrackerTest.cc
				
				test/font/TrueTypeFileTest.cc
				test/font/TrueTypeTableTest.cc
				
				test/mock/MockFile.cc
)

if ( PNG_FOUND )
    set( UT_SRCS    ${UT_SRCS}
				    test/image/PngImageTest.cc )
endif ( PNG_FOUND )

if ( JPEG_FOUND )
    set( UT_SRCS    ${UT_SRCS}
				    test/image/JpegImageTest.cc )
endif ( JPEG_FOUND )

if ( FREETYPE_FOUND )
    set( UT_SRCS    ${UT_SRCS}
    				test/font/SimpleFontTest.cc )
endif ( FREETYPE_FOUND )

add_executable( unittest ${UT_SRCS} test/UnitTest.cc )
target_link_libraries( unittest pdfdoc_static ${CPPUNIT_LIBRARY} )

else ( CPPUNIT_FOUND )
	message( STATUS "skip building unittest" )
endif ( CPPUNIT_FOUND )
