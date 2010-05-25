#***************************************************************************#
 #   Copyright (C) 2002 by Nestal Wan                                      *
 #   me@nestal.net                                                         *
 #                                                                         *
 #   This program is free software; you can redistribute it and/or modify  *
 #   it under the terms of the GNU General Public License as published by  *
 #   the Free Software Foundation; either version 2 of the License.        *
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

set( VERSION	0.2.1 )
add_definitions( -DVERSION="${VERSION}" )
add_definitions( -DBOOST_ALL_NO_LIB )

###############################################################################
# required packages
###############################################################################
set( Boost_USE_MULTITHREAD ON )
find_package( Boost REQUIRED )
find_package( ZLIB REQUIRED )
find_package( Freetype REQUIRED )

###############################################################################
# optional packages
###############################################################################
find_package( PNG )
find_package( JPEG )

###############################################################################
# finding optional fontconfig library
###############################################################################
find_path( FONTCONFIG_INCLUDE_DIR
	fontconfig/fontconfig.h
	/usr/include
	/usr/local/include
	${FONTCONFIG_PREFIX}/include )

find_library( FONTCONFIG_LIBRARY_DEBUG NAMES fontconfig
	PATHS
		/usr/lib
    	/usr/lib64
        /usr/local/lib
        /usr/local/lib64
        ${FONTCONFIG_PREFIX}/lib
	PATH_SUFFIXES debug )

find_library( FONTCONFIG_LIBRARY_RELEASE NAMES fontconfig
	PATHS
		/usr/lib
    	/usr/lib64
        /usr/local/lib
        /usr/local/lib64
        ${FONTCONFIG_PREFIX}/lib
	PATH_SUFFIXES release )

set( FONTCONFIG_LIBRARY debug	${FONTCONFIG_LIBRARY_DEBUG}
                    optimized	${FONTCONFIG_LIBRARY_RELEASE} )

if ( FONTCONFIG_INCLUDE_DIR AND FONTCONFIG_LIBRARY )
	message( STATUS "found fontconfig" )
	set( FONTCONFIG_FOUND TRUE )
endif ( FONTCONFIG_INCLUDE_DIR AND FONTCONFIG_LIBRARY )

###############################################################################
# source subdirectories
###############################################################################
set( PAGE_DIR		src/page/ )
set( GRAPICS_DIR	src/graphics/ )
set( FILE_DIR		src/file/ )
set( CORE_DIR		src/core/ )
set( STRM_DIR		src/stream/ )
set( IMG_DIR		src/image/ )
set( UTIL_DIR		src/util/ )
set( FONT_DIR		src/font/ )
set( INCLUDE_DIR	include/ )

###############################################################################
# headers
###############################################################################
set( PAGE_HDRS		${PAGE_DIR}/RealPage.hh
					${PAGE_DIR}/ResourcesDict.hh
					${PAGE_DIR}/RealResources.hh
					${PAGE_DIR}/MockResources.hh
					${PAGE_DIR}/ResourceSet.hh
					${PAGE_DIR}/PageNode.hh
					${PAGE_DIR}/PageTree.hh
					${PAGE_DIR}/PageInfo.hh
					${PAGE_DIR}/ContentStream.hh
					${PAGE_DIR}/ContentOp.hh
					${PAGE_DIR}/ExtGState.hh )

set( GRAPICS_HDRS	${GRAPICS_DIR}/RealText.hh
					${GRAPICS_DIR}/RealPath.hh
					${GRAPICS_DIR}/RealImage.hh
					${GRAPICS_DIR}/XObject.hh
					${GRAPICS_DIR}/RealGraphicsGroup.hh
					${GRAPICS_DIR}/RealColorSpace.hh )

set( FILE_HDRS		${FILE_DIR}/RealFile.hh
					${FILE_DIR}/File.hh
					${FILE_DIR}/DictReader.hh
					${FILE_DIR}/ArrayReader.hh
					${FILE_DIR}/Catalog.hh
					${FILE_DIR}/Destination.hh
					${FILE_DIR}/NameTree.hh
					${FILE_DIR}/ElementPool.hh
					${FILE_DIR}/Function.hh
					${FILE_DIR}/ElementFactory.hh )

set( CORE_HDRS  	${CORE_DIR}/Object.hh
					${CORE_DIR}/Ref.hh
					${CORE_DIR}/Array.hh
					${CORE_DIR}/Dictionary.hh
					${CORE_DIR}/Token.hh
					${CORE_DIR}/String.hh
					${CORE_DIR}/Name.hh
					${CORE_DIR}/TraverseObject.hh )
					
set( STRM_HDRS		${STRM_DIR}/Stream.hh
					
					# stream filters
					${STRM_DIR}/StreamFilter.hh
					${STRM_DIR}/StreamError.hh
					${STRM_DIR}/RawFilter.hh
					${STRM_DIR}/DeflateFilter.hh
					${STRM_DIR}/BufferedFilter.hh
					${STRM_DIR}/MockStreamFilter.hh
					${STRM_DIR}/InStreamBufAdaptor.hh
					${STRM_DIR}/OutStreamBufAdaptor.hh )

set( UTIL_HDRS  	${UTIL_DIR}/Export.hh
					${UTIL_DIR}/SymbolInfo.hh
					${UTIL_DIR}/Backtrace.hh
					${UTIL_DIR}/RefCounter.hh
					${UTIL_DIR}/RefPtr.hh )
					
set( IMG_HDRS  		${IMG_DIR}/Pixmap.hh
					${IMG_DIR}/PngImage.hh
					${IMG_DIR}/JpegImage.hh )

set( FONT_HDRS		${FONT_DIR}/Types.hh
					${FONT_DIR}/BaseFont.hh 
                    ${FONT_DIR}/SimpleFont.hh
                    ${FONT_DIR}/CompositeFont.hh
					${FONT_DIR}/FontDescriptor.hh
					${FONT_DIR}/FontException.hh
					${FONT_DIR}/RealGlyph.hh
					${FONT_DIR}/BasicFontDb.hh
					${FONT_DIR}/FontCache.hh
					${FONT_DIR}/encoding/AdobeCMap.hh
					${FONT_DIR}/encoding/CodeMap.hh
					${FONT_DIR}/encoding/BaseEncoding.hh
					${FONT_DIR}/encoding/SimpleEncoding.hh
					${FONT_DIR}/encoding/Type1Encoding.hh
					${FONT_DIR}/encoding/BuildInEncoding.hh
					${FONT_DIR}/FontSubsetInfo.hh
					${FONT_DIR}/subset/Sfnt.hh
					${FONT_DIR}/subset/Types.hh
					${FONT_DIR}/subset/ReadStream.hh
					${FONT_DIR}/subset/WriteStream.hh
					${FONT_DIR}/subset/Endian.hh )

set( BASE_HDRS		src/RealDoc.hh )

###############################################################################
# exported header files
###############################################################################

set( INCLUDE_HDRS	${INCLUDE_DIR}/Doc.hh
					${INCLUDE_DIR}/libpdfdoc.hh
					${INCLUDE_DIR}/DocInfo.hh )

set( FONT_INC_HDRS	${INCLUDE_DIR}/font/Font.hh
					${INCLUDE_DIR}/font/Glyph.hh
					${INCLUDE_DIR}/font/FontDb.hh
					${INCLUDE_DIR}/font/FontType.hh
					${INCLUDE_DIR}/font/Outline.hh
					${INCLUDE_DIR}/font/FontEncoding.hh )

set( PAGE_INC_HDRS	${INCLUDE_DIR}/page/Page.hh )

set( GFX_INC_HDRS	${INCLUDE_DIR}/graphics/Graphics.hh
					${INCLUDE_DIR}/graphics/GraphicsVisitor.hh
					${INCLUDE_DIR}/graphics/GraphicsState.hh
					${INCLUDE_DIR}/graphics/Text.hh
					${INCLUDE_DIR}/graphics/Path.hh
					${INCLUDE_DIR}/graphics/Image.hh
					${INCLUDE_DIR}/graphics/GraphicsGroup.hh
					${INCLUDE_DIR}/graphics/ExtGraphicsLink.hh
					${INCLUDE_DIR}/graphics/PathSegment.hh
					${INCLUDE_DIR}/graphics/TextState.hh
					${INCLUDE_DIR}/graphics/CharVisitor.hh
					${INCLUDE_DIR}/graphics/TextLine.hh
					${INCLUDE_DIR}/graphics/Color.hh
					${INCLUDE_DIR}/graphics/ColorSpace.hh )

set( UTIL_INC_HDRS	${INCLUDE_DIR}/util/Exception.hh
					${INCLUDE_DIR}/util/Debug.hh
					${INCLUDE_DIR}/util/Rect.hh
					${INCLUDE_DIR}/util/Matrix.hh
					${INCLUDE_DIR}/util/Util.hh )

###############################################################################
# source in subdirs
###############################################################################
set( PAGE_SRCS  	${PAGE_DIR}/RealPage.cc
					${PAGE_DIR}/ResourcesDict.cc
					${PAGE_DIR}/RealResources.cc
					${PAGE_DIR}/ResourceSet.cc
					${PAGE_DIR}/MockResources.cc
					${PAGE_DIR}/Page.cc
					${PAGE_DIR}/PageNode.cc
					${PAGE_DIR}/PageTree.cc
					${PAGE_DIR}/PageInfo.cc
					${PAGE_DIR}/ContentStream.cc
					${PAGE_DIR}/ContentOp.cc
					${PAGE_DIR}/ExtGState.cc )

set( GRAPICS_SRCS	${GRAPICS_DIR}/Graphics.cc
					${GRAPICS_DIR}/GraphicsVisitor.cc
					${GRAPICS_DIR}/GraphicsState.cc
					${GRAPICS_DIR}/Text.cc
					${GRAPICS_DIR}/RealText.cc
					${GRAPICS_DIR}/Path.cc
					${GRAPICS_DIR}/Image.cc
					${GRAPICS_DIR}/RealImage.cc
					${GRAPICS_DIR}/ExtGraphicsLink.cc
					${GRAPICS_DIR}/PathSegment.cc
					${GRAPICS_DIR}/RealPath.cc
					${GRAPICS_DIR}/GraphicsGroup.cc
					${GRAPICS_DIR}/RealGraphicsGroup.cc
					${GRAPICS_DIR}/CharVisitor.cc
					${GRAPICS_DIR}/TextLine.cc
					${GRAPICS_DIR}/TextState.cc
					${GRAPICS_DIR}/Color.cc
					${GRAPICS_DIR}/ColorSpace.cc
					${GRAPICS_DIR}/RealColorSpace.cc
					${GRAPICS_DIR}/CreateXObject.cc )

set( FILE_SRCS		${FILE_DIR}/RealFile.cc
					${FILE_DIR}/DictReader.cc
					${FILE_DIR}/ArrayReader.cc
					${FILE_DIR}/File.cc
					${FILE_DIR}/ElementPool.cc
					${FILE_DIR}/Catalog.cc
					${FILE_DIR}/Destination.cc
					${FILE_DIR}/NameTree.cc
					${FILE_DIR}/DocInfo.cc
					${FILE_DIR}/Function.cc )

set( CORE_SRCS  	${CORE_DIR}/Object.cc
					${CORE_DIR}/Ref.cc
					${CORE_DIR}/Array.cc
					${CORE_DIR}/Dictionary.cc
					${CORE_DIR}/Token.cc
					${CORE_DIR}/String.cc
					${CORE_DIR}/Name.cc )

set( STRM_SRCS		${STRM_DIR}/Stream.cc
					
					# stream filters
					${STRM_DIR}/StreamFilter.cc
					${STRM_DIR}/StreamError.cc
					${STRM_DIR}/RawFilter.cc
					${STRM_DIR}/DeflateFilter.cc
					${STRM_DIR}/BufferedFilter.cc
					${STRM_DIR}/MockStreamFilter.cc
					${STRM_DIR}/InStreamBufAdaptor.cc
					${STRM_DIR}/OutStreamBufAdaptor.cc )

set( UTIL_SRCS  	${UTIL_DIR}/Rect.cc
					${UTIL_DIR}/Util.cc
					${UTIL_DIR}/Exception.cc
					${UTIL_DIR}/Backtrace.cc
					${UTIL_DIR}/RefCounter.cc
					${UTIL_DIR}/RefPtr.cc
					${UTIL_DIR}/Debug.cc
					${UTIL_DIR}/Matrix.cc )

set( IMG_SRCS  		${IMG_DIR}/Pixmap.cc )

set( FONT_SRCS		${FONT_DIR}/Font.cc
                    ${FONT_DIR}/SimpleFont.cc
                    ${FONT_DIR}/CompositeFont.cc
                    ${FONT_DIR}/CreateFont.cc
					${FONT_DIR}/Glyph.cc
					${FONT_DIR}/RealGlyph.cc
					${FONT_DIR}/FontDescriptor.cc
					${FONT_DIR}/FontException.cc
					${FONT_DIR}/FontType.cc
					${FONT_DIR}/Outline.cc
					${FONT_DIR}/FontDb.cc
					${FONT_DIR}/BasicFontDb.cc
					${FONT_DIR}/FontCache.cc
					${FONT_DIR}/encoding/AdobeCMap.cc
					${FONT_DIR}/encoding/CodeMap.cc
					${FONT_DIR}/encoding/FontEncoding.cc
					${FONT_DIR}/encoding/BaseEncoding.cc
					${FONT_DIR}/encoding/SimpleEncoding.cc
					${FONT_DIR}/encoding/Type1Encoding.cc
					${FONT_DIR}/encoding/BuildInEncoding.cc
					${FONT_DIR}/FontSubsetInfo.cc
					${FONT_DIR}/subset/Sfnt.cc
					${FONT_DIR}/subset/ReadStream.cc
					${FONT_DIR}/subset/WriteStream.cc
					${FONT_DIR}/subset/Endian.cc )

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
	FIND_PATH( DBG_INCLUDE_DIR dbghelp.h /usr/include /usr/local/include )

	if ( DBG_INCLUDE_DIR )
		add_definitions( -DHAVE_DBGHELP )
	endif ( DBG_INCLUDE_DIR )
	set( UTIL_SRCS  ${UTIL_SRCS}	${UTIL_DIR}/SymbolInfo32.cpp )

    set( FONT_HDRS  ${FONT_HDRS}
					${FONT_DIR}/Win32FontDb.hpp )
    set( FONT_SRCS  ${FONT_SRCS}
					${FONT_DIR}/Win32FontDb.cpp )
endif ( WIN32 )

###############################################################################
# optional sources
###############################################################################

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

# fontconfig
if ( FONTCONFIG_FOUND )
	set( FONT_SRCS	${FONT_SRCS}
					${FONT_DIR}/FCFontDb.cc )
	set( FONT_HDRS	${FONT_HDRS}
					${FONT_DIR}/FCFontDb.hh )
	set( OPT_INCS	${OPT_INCS}
	                ${FONTCONFIG_INCLUDE_DIR} )
	add_definitions( -DHAVE_FONTCONFIG )
	set( OPT_LIBS   ${OPT_LIBS} ${FONTCONFIG_LIBRARY} )	
endif ( FONTCONFIG_FOUND )

###############################################################################
# all headers
###############################################################################
set( HDRS	${BASE_HDRS}
			${PAGE_HDRS} ${FILE_HDRS} ${CORE_HDRS} ${IMG_HDRS}
			${FONT_HDRS} ${STRM_HDRS} ${INCLUDE_HDRS} ${GRAPICS_HDRS} )

###############################################################################
# all sources
###############################################################################
set( SRCS	${BASE_SRCS}
			${PAGE_SRCS} ${FILE_SRCS} ${CORE_SRCS} ${IMG_SRCS} ${UTIL_SRCS}
			${FONT_SRCS} ${STRM_SRCS} ${GRAPICS_SRCS} )

###############################################################################
# libpdfdoc library
###############################################################################
#add_library( pdfdoc SHARED ${SRCS} )
add_library( pdfdoc_static STATIC ${SRCS} ${HDRS} )

###############################################################################
# source groups
###############################################################################

SOURCE_GROUP( "page"		FILES ${PAGE_SRCS}		${PAGE_HDRS} )
SOURCE_GROUP( "graphics"	FILES ${GRAPICS_SRCS}	${GRAPICS_HDRS} )
SOURCE_GROUP( "file"		FILES ${FILE_SRCS}		${FILE_HDRS} )
SOURCE_GROUP( "core"		FILES ${CORE_SRCS}		${CORE_HDRS} )
SOURCE_GROUP( "util"		FILES ${UTIL_SRCS}		${UTIL_HDRS} )
SOURCE_GROUP( "font"		FILES ${FONT_SRCS}		${FONT_HDRS} )
SOURCE_GROUP( "image"		FILES ${IMG_SRCS}  		${IMG_HDRS} )
SOURCE_GROUP( "stream"		FILES ${STRM_SRCS}		${STRM_HDRS} )
SOURCE_GROUP( "base"		FILES ${BASE_SRCS}		${BASE_HDRS} )
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
	find_library( DBHELP_LIBRARY NAMES DbgHelp
	PATHS
		/usr/lib
    	/usr/lib64
        /usr/local/lib
        /usr/local/lib64 )
	
	if ( DBHELP_LIBRARY )
		message( "found dbghelp: ${DBHELP_LIBRARY}" )
		set( OPT_LIBS   ${OPT_LIBS}	Dbghelp )
	else ( DBHELP_LIBRARY )
		message( "dbghelp not found" )
	endif ( DBHELP_LIBRARY )
endif ( WIN32 )

###############################################################################
# all libraries
###############################################################################
target_link_libraries( pdfdoc_static
                       ${ZLIB_LIBRARIES}
                       ${FREETYPE_LIBRARIES}
                       ${OPT_LIBS} )

###############################################################################
# compiler settings
###############################################################################

# to enable export in DLL
add_definitions(-DDLL_EXPORTS)

# for unit test to find test data
add_definitions(-DTEST_DATA_DIR="${libpdfdoc_SOURCE_DIR}/test/testdata/")

# for including header files in the project
add_definitions(-DSRC_DIR="${libpdfdoc_SOURCE_DIR}/src/")

# MSVC has stupid warnings on STD
if ( MSVC )
	add_definitions( -D_SCL_SECURE_NO_WARNINGS -DNOMINMAX )
	set_target_properties( pdfdoc_static
		PROPERTIES COMPILE_FLAGS "/wd4345 /wd4819 /Zc:wchar_t-" )
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
                     ${FREETYPE_INCLUDE_DIRS}
                     ${OPT_INCS} )

INSTALL( TARGETS pdfdoc_static
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib )

INSTALL( FILES ${INCLUDE_HDRS}	DESTINATION include )
INSTALL( FILES ${FONT_INC_HDRS} DESTINATION include/font )
INSTALL( FILES ${PAGE_INC_HDRS} DESTINATION include/page )
INSTALL( FILES ${GFX_INC_HDRS}  DESTINATION include/graphics )
INSTALL( FILES ${UTIL_INC_HDRS} DESTINATION include/util )

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
				test/core/NameTest.cc
				test/core/RefTest.cc
				test/core/ObjectTest.cc
				test/core/StringTest.cc
				test/core/DictionaryTest.cc
				test/core/ArrayTest.cc
				
				test/graphics/TextStateTest.cc
				test/graphics/TextLineTest.cc
				test/graphics/RealTextTest.cc
				test/graphics/GraphicsStateTest.cc
				test/graphics/RealPathTest.cc
				test/graphics/ColorSpaceTest.cc
				
				test/stream/StreamTest.cc
				test/stream/RawFilterTest.cc
				test/stream/DeflateFilterTest.cc
				test/stream/InStreamBufAdaptorTest.cc
				test/stream/OutStreamBufAdaptorTest.cc
				
				test/page/RealPageTest.cc
				test/page/ContentStreamTest.cc
				test/page/PageNodeTest.cc
				test/page/RealResourcesTest.cc
				test/page/ExtGStateTest.cc
				test/page/ResourceSetTest.cc
				
				test/file/RealFileTest.cc
				test/file/CatalogTest.cc
				test/file/NameTreeTest.cc
				test/file/ElementPoolTest.cc
				test/file/DictReaderTest.cc
				test/file/ArrayReaderTest.cc
				test/file/FunctionTest.cc
				
				test/util/MatrixTest.cc
				test/font/FontDbTest.cc
				test/font/CodeMapTest.cc
				test/font/FontEncodingTest.cc
				test/font/CompositeFontTest.cc
				test/font/subset/SfntTest.cc
			
				test/mock/MockFile.cc
				test/mock/MockFont.cc
				test/mock/MockGlyph.cc
				test/mock/TestBase.cc
				test/mock/MockFontDb.cc
				test/mock/MockGraphicsVisitor.cc
				test/RealDocTest.cc
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

# MSVC has stupid warnings on STD
if ( MSVC )
	set_target_properties( unittest
		PROPERTIES COMPILE_FLAGS "/wd4345 /wd4819 /Zc:wchar_t-" )
endif ( MSVC )

else ( CPPUNIT_FOUND )
	message( STATUS "skip building unittest" )
endif ( CPPUNIT_FOUND )
