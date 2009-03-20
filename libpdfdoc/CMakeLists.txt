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

set( Boost_USE_MULTITHREAD ON )
find_package( Boost REQUIRED)
find_package( Freetype )
find_package( ZLIB )
find_package( PNG )
find_package( JPEG )

set( PAGE_DIR		src/page/ )
set( FILE_DIR		src/file/ )
set( CORE_DIR		src/core/ )
set( IMG_DIR		src/image/ )
set( UTIL_DIR		src/util/ )
set( FONT_DIR		src/font/ )
set( FT_DIR			${FONT_DIR}/ftwrap )

# headers
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
					${FILE_DIR}/IElement.hh
					${FILE_DIR}/IElementSrc.hh
					${FILE_DIR}/ElementSrc.hh
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
					${CORE_DIR}/Stream.hh
					${CORE_DIR}/TraverseObject.hh
					
					# stream filters
					${CORE_DIR}/filter/StreamFilter.hh
					${CORE_DIR}/filter/RawFilter.hh
					${CORE_DIR}/filter/DeflateFilter.hh
					${CORE_DIR}/filter/FilterIOStream.hh )

set( UTIL_HDRS  	${UTIL_DIR}/Rect.hh
					${UTIL_DIR}/Export.hh
					${UTIL_DIR}/Util.hh
					${UTIL_DIR}/Exception.hh
					${UTIL_DIR}/SymbolInfo.hh )
					
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

# source in subdirs
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
					${FILE_DIR}/IElement.cc
					${FILE_DIR}/IElementSrc.cc
					${FILE_DIR}/ElementSrc.cc
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
					${CORE_DIR}/Name.cc
					${CORE_DIR}/Stream.cc
					
					# stream filters
					${CORE_DIR}/filter/StreamFilter.cc
					${CORE_DIR}/filter/RawFilter.cc
					${CORE_DIR}/filter/DeflateFilter.cc
					${CORE_DIR}/filter/FilterIOStream.cc )

set( UTIL_SRCS  	${UTIL_DIR}/Rect.cc
					${UTIL_DIR}/Util.cc
					${UTIL_DIR}/Exception.cc )

set( IMG_SRCS  		${IMG_DIR}/Image.cc )

set( FONT_SRCS		${FONT_DIR}/TrueTypeFile.cc
					${FONT_DIR}/TrueTypeTable.cc
					${FONT_DIR}/BaseFont.cc
					${FONT_DIR}/StandardFont.cc )

###############################################################################
# unix specific sources
###############################################################################
if ( UNIX )
    set( UTIL_SRCS  ${UTIL_SRCS}
					${UTIL_DIR}/SymbolInfo.cc )
###############################################################################
# win32 specific sources
###############################################################################
elseif ( WIN32)
    set( UTIL_SRCS  ${UTIL_SRCS}
					${UTIL_DIR}/SymbolInfo32.cpp )
endif (UNIX )

###############################################################################
# win32 optional sources
###############################################################################
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
    set( OPT_LIBS   ${OPT_LIBS}
                    ${FREETYPE_LIBRARIES} )
    set( OPT_INCS   ${OPT_INCS}
                    ${FREETYPE_INCLUDE_DIRS} )
    add_definitions( -DHAVE_FREETYPE )
endif ( FREETYPE_FOUND )

if ( PNG_FOUND )
    set( IMG_SRCS   ${IMG_SRCS}
                    ${IMG_DIR}/PngImage.cc )
    set( IMG_HDRS   ${IMG_HDRS}
                    ${IMG_DIR}/PngImage.hh )
    set( OPT_LIBS   ${OPT_LIBS}
                    ${PNG_LIBRARIES} )
    set( OPT_INCS   ${OPT_INCS}
                    ${PNG_INCLUDE_DIR} )
endif ( PNG_FOUND )

if ( JPEG_FOUND )
    set( IMG_SRCS   ${IMG_SRCS}
                    ${IMG_DIR}/JpegImage.cc )
    set( IMG_HDRS   ${IMG_HDRS}
                    ${IMG_DIR}/JpegImage.hh )
    set( OPT_LIBS   ${OPT_LIBS}
                    ${JPEG_LIBRARIES} )
    set( OPT_INCS   ${OPT_INCS}
                    ${JPEG_INCLUDE_DIR} )
endif ( JPEG_FOUND )

###############################################################################
# all headers
###############################################################################
set( HDRS	src/RealDoc.hh
			${PAGE_HDRS} ${FILE_HDRS} ${CORE_HDRS} ${IMG_HDRS}
			${FONT_HDRS} ${FT_HDRS} )

###############################################################################
# all sources
###############################################################################
set( SRCS	src/Doc.cc src/RealDoc.cc src/libpdfdoc.cc
			${PAGE_SRCS} ${FILE_SRCS} ${CORE_SRCS} ${IMG_SRCS} ${UTIL_SRCS}
			${FONT_SRCS} ${FT_SRCS} )

###############################################################################
# libpdfdoc library
###############################################################################
#add_library( pdfdoc SHARED ${SRCS} )
add_library( pdfdoc_static STATIC ${SRCS} ${HDRS} )

###############################################################################
# source groups
###############################################################################
SOURCE_GROUP( "Source Files\\page" FILES ${PAGE_SRCS} )
SOURCE_GROUP( "Source Files\\file" FILES ${FILE_SRCS} )
SOURCE_GROUP( "Source Files\\core" FILES ${CORE_SRCS} )
SOURCE_GROUP( "Source Files\\util" FILES ${UTIL_SRCS} )
SOURCE_GROUP( "Source Files\\font" FILES ${FONT_SRCS} )
SOURCE_GROUP( "Source Files\\image" FILES ${IMG_SRCS} )
SOURCE_GROUP( "Header Files\\page" FILES ${PAGE_HDRS} )
SOURCE_GROUP( "Header Files\\file" FILES ${FILE_HDRS} )
SOURCE_GROUP( "Header Files\\core" FILES ${CORE_HDRS} )
SOURCE_GROUP( "Header Files\\util" FILES ${UTIL_HDRS} )
SOURCE_GROUP( "Header Files\\font" FILES ${FONT_HDRS} )
SOURCE_GROUP( "Header Files\\image" FILES ${IMG_HDRS} )

IF ( MSVC )
	ADD_DEFINITIONS( -D_SCL_SECURE_NO_WARNINGS -DDLL_EXPORTS )
ELSE ( MSVC )
	ADD_DEFINITIONS( -DDLL_EXPORTS )
ENDIF ( MSVC )

###############################################################################
# unix specific libraries
###############################################################################
if ( UNIX )
    set( OPT_LIBS   ${OPT_LIBS}
                    dl bfd )

###############################################################################
# win32 specific libraries
###############################################################################
elseif ( WIN32 )
    set( OPT_LIBS   ${OPT_LIBS}
                    Dbghelp )
endif ( WIN32 )


###############################################################################
# all compiler settings
###############################################################################
add_definitions( -DDLL_EXPORTS )

target_link_libraries( pdfdoc_static
                       ${ZLIB_LIBRARIES}
                       ${OPT_LIBS} )

include_directories( ${libpdfdoc_SOURCE_DIR}/src
                     ${libpdfdoc_SOURCE_DIR}/include
                     ${libpdfdoc_SOURCE_DIR}/test
                     ${ZLIB_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     ${CPPUNIT_PREFIX}/include
                     ${OPT_INCS} )

INSTALL( TARGETS pdfdoc_static
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib )

INSTALL( FILES ${HDRS} DESTINATION include )

###############################################################################
# test driver
###############################################################################
add_executable( testpdf src/testpdf.cc )
target_link_libraries( testpdf pdfdoc_static )

###############################################################################
# unit test sources
###############################################################################
set( UT_SRCS	test/core/TokenTest.cc
				test/core/NameTest.cc
				test/core/RefTest.cc
				test/core/ObjectTest.cc
				test/core/StringTest.cc
				test/core/DictionaryTest.cc
				test/core/ArrayTest.cc
				test/core/StreamTest.cc
				test/core/filter/RawFilterTest.cc
				test/core/filter/DeflateFilterTest.cc
				test/core/filter/FilterIOStreamTest.cc
				
				test/page/PageTest.cc
				test/page/PageNodeTest.cc
				test/page/ResourcesTest.cc
				test/page/XObjectTest.cc
				
				test/file/FileTest.cc
				test/ElementTrackerTest.cc
				
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

if ( CPPUNIT_PREFIX )
    link_directories( ${CPPUNIT_PREFIX}/lib )
endif ( CPPUNIT_PREFIX )

add_definitions(-DTEST_DATA_DIR="${libpdfdoc_SOURCE_DIR}/test/testdata/")
add_definitions(-DSRC_DIR="\\"${libpdfdoc_SOURCE_DIR}/src/\\"")
add_executable( unittest ${UT_SRCS} test/UnitTest.cc )
target_link_libraries( unittest pdfdoc_static cppunit )
