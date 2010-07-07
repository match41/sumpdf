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
	\file	CCITTFaxStream.cc
	\brief	implementation the StreamFilter class
	\date	Wed Mar 4 2009
	\author	Nestal Wan
*/
#include "CCITTFaxStream.hh"

#include "Stream-CCITT.h"

#include "core/Array.hh"
#include "core/Name.hh"
#include "core/Object.hh"

#include <cstdlib>
#include <climits>

namespace xpdf {

using namespace pdf ;

namespace
{
	const bool gTrue	= true ;
	const bool gFalse	= false ;

	#define gmallocn	std::calloc
	#define gfree		std::free

	int getPos( ... )
	{
		return 0 ;
	}

	void error( ... )
	{
		throw -1 ;
	}
}

std::size_t CCITTFaxStream::Read( unsigned char *data, std::size_t size )
{
	std::size_t count ;
	for ( count = 0 ; count < size ; ++count )
	{
		int c = getChar() ;
		if ( c == EOF )
			break ;
		
		data[count] = static_cast<unsigned char>(c) ;
	}
	return count ;
}

std::size_t CCITTFaxStream::Write( const unsigned char *data, std::size_t size )
{
	// write is not supported. pretend to be working.
	return size ;
}

void CCITTFaxStream::Flush( )
{
}

std::size_t CCITTFaxStream::Length( ) const
{
	return m_src->Length() ;
}

Object CCITTFaxStream::NameChain( ) const
{
	Object name = m_src->NameChain( ) ;
	if ( name.Is<Array>( ) )
	{
		Array arr ;
		arr.push_back( Name( "CCITTFaxDecode" ) ) ;
		
		const Array& old = name.As<Array>( ) ;
		arr.insert( arr.end(), old.begin(), old.end() ) ;
		return Object( arr ) ;
	}
	else if ( name.Is<void>() )
		return Name( "CCITTFaxDecode" ) ;
	else
		throw Exception( "invalid filter" ) ;
}

StreamFilter* CCITTFaxStream::Clone( ) const
{
	return 0 ;
}

StreamFilter* CCITTFaxStream::GetInner( )
{
	return m_src.get() ;
}

Name CCITTFaxStream::RawFormat( ) const
{
	return Name() ;
}

int CCITTFaxStream::ReadChar( )
{
	unsigned char byte ;
	std::size_t s = m_src->Read( &byte, 1 ) ;
	return s == 1 ? byte : EOF ;
}

/////////////////////////////////////////////////////
// xpdf code

CCITTFaxStream::CCITTFaxStream(std::auto_ptr<pdf::StreamFilter> strA, int encodingA, GBool endOfLineA,
			       GBool byteAlignA, int columnsA, int rowsA,
			       GBool endOfBlockA, GBool blackA):
    m_src(strA) {
  encoding = encodingA;
  endOfLine = endOfLineA;
  byteAlign = byteAlignA;
  columns = columnsA;
  if (columns < 1) {
    columns = 1;
  }
  if (columns + 4 <= 0) {
    columns = INT_MAX - 4;
  }
  rows = rowsA;
  endOfBlock = endOfBlockA;
  black = blackA;
  refLine = (short *)gmallocn(columns + 3, sizeof(short));
  codingLine = (short *)gmallocn(columns + 2, sizeof(short));

  eof = gFalse;
  row = 0;
  nextLine2D = encoding < 0;
  inputBits = 0;
  codingLine[0] = 0;
  codingLine[1] = refLine[2] = columns;
  a0 = 1;

  buf = EOF;
}

CCITTFaxStream::~CCITTFaxStream() {

  gfree(refLine);
  gfree(codingLine);
}

void CCITTFaxStream::Rewind()
{
  short code1;

  m_src->Rewind();
  eof = gFalse;
  row = 0;
  nextLine2D = encoding < 0;
  inputBits = 0;
  codingLine[0] = 0;
  codingLine[1] = columns;
  a0 = 1;
  buf = EOF;

  // skip any initial zero bits and end-of-line marker, and get the 2D
  // encoding tag
  while ((code1 = lookBits(12)) == 0) {
    eatBits(1);
  }
  if (code1 == 0x001) {
    eatBits(12);
  }
  if (encoding > 0) {
    nextLine2D = !lookBits(1);
    eatBits(1);
  }
}

int CCITTFaxStream::lookChar() {
  short code1, code2, code3;
  int a0New;
  GBool err, gotEOL;
  int ret;
  int bits, i;

  // if at eof just return EOF
  if (eof && codingLine[a0] >= columns) {
    return EOF;
  }

  // read the next row
  err = gFalse;
  if (codingLine[a0] >= columns) {

    // 2-D encoding
    if (nextLine2D) {
      // state:
      //   a0New = current position in coding line (0 <= a0New <= columns)
      //   codingLine[a0] = last change in coding line
      //                    (black-to-white if a0 is even,
      //                     white-to-black if a0 is odd)
      //   refLine[b1] = next change in reference line of opposite color
      //                 to a0
      // invariants:
      //   0 <= codingLine[a0] <= a0New
      //           <= refLine[b1] <= refLine[b1+1] <= columns
      //   0 <= a0 <= columns+1
      //   refLine[0] = 0
      //   refLine[n] = refLine[n+1] = columns
      //     -- for some 1 <= n <= columns+1
      // end condition:
      //   0 = codingLine[0] <= codingLine[1] < codingLine[2] < ...
      //     < codingLine[n-1] < codingLine[n] = columns
      //     -- where 1 <= n <= columns+1
      for (i = 0; codingLine[i] < columns; ++i) {
	refLine[i] = codingLine[i];
      }
      refLine[i] = refLine[i + 1] = columns;
      b1 = 1;
      a0New = codingLine[a0 = 0] = 0;
      do {
	code1 = getTwoDimCode();
	switch (code1) {
	case twoDimPass:
	  if (refLine[b1] < columns) {
	    a0New = refLine[b1 + 1];
	    b1 += 2;
	  }
	  break;
	case twoDimHoriz:
	  if ((a0 & 1) == 0) {
	    code1 = code2 = 0;
	    do {
	      code1 += code3 = getWhiteCode();
	    } while (code3 >= 64);
	    do {
	      code2 += code3 = getBlackCode();
	    } while (code3 >= 64);
	  } else {
	    code1 = code2 = 0;
	    do {
	      code1 += code3 = getBlackCode();
	    } while (code3 >= 64);
	    do {
	      code2 += code3 = getWhiteCode();
	    } while (code3 >= 64);
	  }
	  if (code1 > 0 || code2 > 0) {
	    if (a0New + code1 <= columns) {
	      codingLine[a0 + 1] = a0New + code1;
	    } else {
	      codingLine[a0 + 1] = columns;
	    }
	    ++a0;
	    if (codingLine[a0] + code2 <= columns) {
	      codingLine[a0 + 1] = codingLine[a0] + code2;
	    } else {
	      codingLine[a0 + 1] = columns;
	    }
	    ++a0;
	    a0New = codingLine[a0];
	    while (refLine[b1] <= a0New && refLine[b1] < columns) {
	      b1 += 2;
	    }
	  }
	  break;
	case twoDimVert0:
	  if (refLine[b1] < columns) {
	    a0New = codingLine[++a0] = refLine[b1];
	    ++b1;
	    while (refLine[b1] <= a0New && refLine[b1] < columns) {
	      b1 += 2;
	    }
	  } else {
	    a0New = codingLine[++a0] = columns;
	  }
	  break;
	case twoDimVertR1:
	  if (refLine[b1] + 1 < columns) {
	    a0New = codingLine[++a0] = refLine[b1] + 1;
	    ++b1;
	    while (refLine[b1] <= a0New && refLine[b1] < columns) {
	      b1 += 2;
	    }
	  } else {
	    a0New = codingLine[++a0] = columns;
	  }
	  break;
	case twoDimVertL1:
	  if (refLine[b1] - 1 > a0New || (a0 == 0 && refLine[b1] == 1)) {
	    a0New = codingLine[++a0] = refLine[b1] - 1;
	    --b1;
	    while (refLine[b1] <= a0New && refLine[b1] < columns) {
	      b1 += 2;
	    }
	  }
	  break;
	case twoDimVertR2:
	  if (refLine[b1] + 2 < columns) {
	    a0New = codingLine[++a0] = refLine[b1] + 2;
	    ++b1;
	    while (refLine[b1] <= a0New && refLine[b1] < columns) {
	      b1 += 2;
	    }
	  } else {
	    a0New = codingLine[++a0] = columns;
	  }
	  break;
	case twoDimVertL2:
	  if (refLine[b1] - 2 > a0New || (a0 == 0 && refLine[b1] == 2)) {
	    a0New = codingLine[++a0] = refLine[b1] - 2;
	    --b1;
	    while (refLine[b1] <= a0New && refLine[b1] < columns) {
	      b1 += 2;
	    }
	  }
	  break;
	case twoDimVertR3:
	  if (refLine[b1] + 3 < columns) {
	    a0New = codingLine[++a0] = refLine[b1] + 3;
	    ++b1;
	    while (refLine[b1] <= a0New && refLine[b1] < columns) {
	      b1 += 2;
	    }
	  } else {
	    a0New = codingLine[++a0] = columns;
	  }
	  break;
	case twoDimVertL3:
	  if (refLine[b1] - 3 > a0New || (a0 == 0 && refLine[b1] == 3)) {
	    a0New = codingLine[++a0] = refLine[b1] - 3;
	    --b1;
	    while (refLine[b1] <= a0New && refLine[b1] < columns) {
	      b1 += 2;
	    }
	  }
	  break;
	case EOF:
	  eof = gTrue;
	  codingLine[a0 = 0] = columns;
	  return EOF;
	default:
	  error(getPos(), "Bad 2D code %04x in CCITTFax stream", code1);
	  err = gTrue;
	  break;
	}
      } while (codingLine[a0] < columns);

    // 1-D encoding
    } else {
      codingLine[a0 = 0] = 0;
      while (1) {
	code1 = 0;
	do {
	  code1 += code3 = getWhiteCode();
	} while (code3 >= 64);
	codingLine[a0+1] = codingLine[a0] + code1;
	++a0;
	if (codingLine[a0] >= columns) {
	  break;
	}
	code2 = 0;
	do {
	  code2 += code3 = getBlackCode();
	} while (code3 >= 64);
	codingLine[a0+1] = codingLine[a0] + code2;
	++a0;
	if (codingLine[a0] >= columns) {
	  break;
	}
      }
    }

    if (codingLine[a0] != columns) {
      error(getPos(), "CCITTFax row is wrong length (%d)", codingLine[a0]);
      // force the row to be the correct length
      while (codingLine[a0] > columns) {
	--a0;
      }
      codingLine[++a0] = columns;
      err = gTrue;
    }

    // byte-align the row
    if (byteAlign) {
      inputBits &= ~7;
    }

    // check for end-of-line marker, skipping over any extra zero bits
    gotEOL = gFalse;
    if (!endOfBlock && row == rows - 1) {
      eof = gTrue;
    } else {
      code1 = lookBits(12);
      while (code1 == 0) {
	eatBits(1);
	code1 = lookBits(12);
      }
      if (code1 == 0x001) {
	eatBits(12);
	gotEOL = gTrue;
      } else if (code1 == EOF) {
	eof = gTrue;
      }
    }

    // get 2D encoding tag
    if (!eof && encoding > 0) {
      nextLine2D = !lookBits(1);
      eatBits(1);
    }

    // check for end-of-block marker
    if (endOfBlock && gotEOL) {
      code1 = lookBits(12);
      if (code1 == 0x001) {
	eatBits(12);
	if (encoding > 0) {
	  lookBits(1);
	  eatBits(1);
	}
	if (encoding >= 0) {
	  for (i = 0; i < 4; ++i) {
	    code1 = lookBits(12);
	    if (code1 != 0x001) {
	      error(getPos(), "Bad RTC code in CCITTFax stream");
	    }
	    eatBits(12);
	    if (encoding > 0) {
	      lookBits(1);
	      eatBits(1);
	    }
	  }
	}
	eof = gTrue;
      }

    // look for an end-of-line marker after an error -- we only do
    // this if we know the stream contains end-of-line markers because
    // the "just plow on" technique tends to work better otherwise
    } else if (err && endOfLine) {
      do {
	if (code1 == EOF) {
	  eof = gTrue;
	  return EOF;
	}
	eatBits(1);
	code1 = lookBits(13);
      } while ((code1 >> 1) != 0x001);
      eatBits(12); 
      if (encoding > 0) {
	eatBits(1);
	nextLine2D = !(code1 & 1);
      }
    }

    a0 = 0;
    outputBits = codingLine[1] - codingLine[0];
    if (outputBits == 0) {
      a0 = 1;
      outputBits = codingLine[2] - codingLine[1];
    }

    ++row;
  }

  // get a byte
  if (outputBits >= 8) {
    ret = ((a0 & 1) == 0) ? 0xff : 0x00;
    if ((outputBits -= 8) == 0) {
      ++a0;
      if (codingLine[a0] < columns) {
	outputBits = codingLine[a0 + 1] - codingLine[a0];
      }
    }
  } else {
    bits = 8;
    ret = 0;
    do {
      if (outputBits > bits) {
	i = bits;
	bits = 0;
	if ((a0 & 1) == 0) {
	  ret |= 0xff >> (8 - i);
	}
	outputBits -= i;
      } else {
	i = outputBits;
	bits -= outputBits;
	if ((a0 & 1) == 0) {
	  ret |= (0xff >> (8 - i)) << bits;
	}
	outputBits = 0;
	++a0;
	if (codingLine[a0] < columns) {
	  outputBits = codingLine[a0 + 1] - codingLine[a0];
	}
      }
    } while (bits > 0 && codingLine[a0] < columns);
  }
  buf = black ? (ret ^ 0xff) : ret;
  return buf;
}

short CCITTFaxStream::getTwoDimCode() {
  short code;
  CCITTCode *p;
  int n;

  code = 0; // make gcc happy
  if (endOfBlock) {
    code = lookBits(7);
    p = &twoDimTab1[code];
    if (p->bits > 0) {
      eatBits(p->bits);
      return p->n;
    }
  } else {
    for (n = 1; n <= 7; ++n) {
      code = lookBits(n);
      if (n < 7) {
	code <<= 7 - n;
      }
      p = &twoDimTab1[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
  }
  error(getPos(), "Bad two dim code (%04x) in CCITTFax stream", code);
  return EOF;
}

short CCITTFaxStream::getWhiteCode() {
  short code;
  CCITTCode *p;
  int n;

  code = 0; // make gcc happy
  if (endOfBlock) {
    code = lookBits(12);
    if ((code >> 5) == 0) {
      p = &whiteTab1[code];
    } else {
      p = &whiteTab2[code >> 3];
    }
    if (p->bits > 0) {
      eatBits(p->bits);
      return p->n;
    }
  } else {
    for (n = 1; n <= 9; ++n) {
      code = lookBits(n);
      if (n < 9) {
	code <<= 9 - n;
      }
      p = &whiteTab2[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
    for (n = 11; n <= 12; ++n) {
      code = lookBits(n);
      if (n < 12) {
	code <<= 12 - n;
      }
      p = &whiteTab1[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
  }
  error(getPos(), "Bad white code (%04x) in CCITTFax stream", code);
  // eat a bit and return a positive number so that the caller doesn't
  // go into an infinite loop
  eatBits(1);
  return 1;
}

short CCITTFaxStream::getBlackCode() {
  short code;
  CCITTCode *p;
  int n;

  code = 0; // make gcc happy
  if (endOfBlock) {
    code = lookBits(13);
    if ((code >> 7) == 0) {
      p = &blackTab1[code];
    } else if ((code >> 9) == 0) {
      p = &blackTab2[(code >> 1) - 64];
    } else {
      p = &blackTab3[code >> 7];
    }
    if (p->bits > 0) {
      eatBits(p->bits);
      return p->n;
    }
  } else {
    for (n = 2; n <= 6; ++n) {
      code = lookBits(n);
      if (n < 6) {
	code <<= 6 - n;
      }
      p = &blackTab3[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
    for (n = 7; n <= 12; ++n) {
      code = lookBits(n);
      if (n < 12) {
	code <<= 12 - n;
      }
      if (code >= 64) {
	p = &blackTab2[code - 64];
	if (p->bits == n) {
	  eatBits(n);
	  return p->n;
	}
      }
    }
    for (n = 10; n <= 13; ++n) {
      code = lookBits(n);
      if (n < 13) {
	code <<= 13 - n;
      }
      p = &blackTab1[code];
      if (p->bits == n) {
	eatBits(n);
	return p->n;
      }
    }
  }
  error(getPos(), "Bad black code (%04x) in CCITTFax stream", code);
  // eat a bit and return a positive number so that the caller doesn't
  // go into an infinite loop
  eatBits(1);
  return 1;
}

short CCITTFaxStream::lookBits(int n) {
  int c;

  while (inputBits < n) {
    if ((c = ReadChar()) == EOF) {
      if (inputBits == 0) {
	return EOF;
      }
      // near the end of the stream, the caller may ask for more bits
      // than are available, but there may still be a valid code in
      // however many bits are available -- we need to return correct
      // data in this case
      return (inputBuf << (n - inputBits)) & (0xffff >> (16 - n));
    }
    inputBuf = (inputBuf << 8) + c;
    inputBits += 8;
  }
  return (inputBuf >> (inputBits - n)) & (0xffff >> (16 - n));
}

} // end of namespace xpdf
