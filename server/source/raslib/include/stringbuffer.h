/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: stringbuffer.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __STRINGBUFFER_H__
#define __STRINGBUFFER_H__

#include "buffer.h"

namespace SDSocket
{
  using std::string;

  class StringBuffer : public Buffer
  {
  public:
    enum { DEFAULT_BUFFER_SIZE = 4096 };

    StringBuffer(const string& text) throw();
    StringBuffer(size_t tempBufferSize = DEFAULT_BUFFER_SIZE) throw();
    ~StringBuffer() throw();

    operator string &() throw();

  protected:
    virtual size_t size() const throw();
    virtual char* pointer() throw();
    virtual const char* pointer() const throw();
    virtual void prepare() throw();
    virtual void update(size_t) throw();

  private:
    // Buffered text
    string buffer;
    // Pointer to temporary buffer.
    char* ptrToBuffer;
    // Size of temporary buffer.
    size_t bufferSize;
  };
}

#endif
