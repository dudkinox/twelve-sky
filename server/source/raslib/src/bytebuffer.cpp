/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: bytebuffer.cpp,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "bytebuffer.h"

namespace SDSocket
{	
  ByteBuffer::ByteBuffer(size_t tempBufferSize) throw()
  {
		pBuffer = new char[tempBufferSize];
		bufferSize = tempBufferSize;
		pTmpBuffer = NULL;
  }
  
  ByteBuffer::~ByteBuffer() throw()
  {
    delete[] pBuffer;
  }
  
  size_t ByteBuffer::size() const throw()
  {
		return bufferSize;
  }

  char* ByteBuffer::pointer() throw()
  {
    return pBuffer;
  }

  void ByteBuffer::resize(const size_t Size) throw()
  {
		pTmpBuffer = new char[bufferSize];
		::memcpy(pTmpBuffer, pBuffer, bufferSize);
		delete[] pBuffer;
		bufferSize += Size;
		pBuffer = new char[bufferSize];
		::memcpy(pBuffer, pTmpBuffer, bufferSize);
		delete[] pTmpBuffer;
  }

  void ByteBuffer::reset() throw()
  {
		::memset(pBuffer, 0, bufferSize);
  }
}
