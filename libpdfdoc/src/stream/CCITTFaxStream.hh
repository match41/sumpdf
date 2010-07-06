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

/**	\file	CCITTFaxStream.hh
    \brief	definition the PredictFilter class
    \date	Jun 25, 2010
    \author	Nestal Wan
*/

#ifndef __PDF_CCITTFAXSTREAM_HEADER_INCLUDED__
#define __PDF_CCITTFAXSTREAM_HEADER_INCLUDED__

#include "StreamFilter.hh"

#include <memory>
#include <cstdio>

namespace xpdf {

// workaround xpdf code
typedef bool GBool ;

class CCITTFaxStream : public pdf::StreamFilter
{
public:

	CCITTFaxStream( std::auto_ptr<pdf::StreamFilter> strA,
		int encodingA, GBool endOfLineA,
		GBool byteAlignA, int columnsA, int rowsA,
		GBool endOfBlockA, GBool blackA);
	~CCITTFaxStream();

	std::size_t Read( unsigned char *data, std::size_t size ) ;
	std::size_t Write( const unsigned char *data, std::size_t size ) ;
	void Flush( ) ;
	void Rewind( ) ;
	std::size_t Length( ) const ;
	pdf::Object NameChain( ) const ;
	pdf::StreamFilter* Clone( ) const ;
	pdf::StreamFilter* GetInner( ) ;
	pdf::Name RawFormat( ) const ;

private:
	const std::auto_ptr<pdf::StreamFilter>	m_src ;
	int ReadChar( ) ;

	int lookChar();
	int getChar()
    { int c = lookChar(); buf = EOF; return c; }

private:

  int encoding;                 // 'K' parameter
  GBool endOfLine;              // 'EndOfLine' parameter
  GBool byteAlign;              // 'EncodedByteAlign' parameter
  int columns;                  // 'Columns' parameter
  int rows;                     // 'Rows' parameter
  GBool endOfBlock;             // 'EndOfBlock' parameter
  GBool black;                  // 'BlackIs1' parameter
  GBool eof;                    // true if at eof
  GBool nextLine2D;             // true if next line uses 2D encoding
  int row;                      // current row
  int inputBuf;                 // input buffer
  int inputBits;                // number of bits in input buffer
  short *refLine;               // reference line changing elements
  int b1;                       // index into refLine
  short *codingLine;            // coding line changing elements
  int a0;                       // index into codingLine
  int outputBits;               // remaining ouput bits
  int buf;                      // character buffer

  short getTwoDimCode();
  short getWhiteCode();
  short getBlackCode();
  short lookBits(int n);
  void eatBits(int n) { if ((inputBits -= n) < 0) inputBits = 0; }
} ;

} // end of namespace

#endif
