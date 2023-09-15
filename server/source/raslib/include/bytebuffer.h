/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: bytebuffer.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __BYTEBUFFER_H__
#define __BYTEBUFFER_H__

#include "buffer.h"

#define DEFAULT_BUFFER_SIZE   9216 
namespace SDSocket
{
  class ByteBuffer
  {
  public:
    

    ByteBuffer(size_t tempBufferSize = DEFAULT_BUFFER_SIZE) throw();
    ~ByteBuffer() throw();
		
    size_t size() const throw();
    char* pointer() throw();
    void resize(const size_t Size) throw();
    void reset() throw();

  private:
    // Buffer Pointer
    char *pBuffer;
  
    // Pointer to temporary buffer.
    char *pTmpBuffer;
  
	// Size of temporary buffer.
    size_t bufferSize;
  };
}

#endif
