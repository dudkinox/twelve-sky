/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: buffer.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <sys/types.h>
#include <string>

#include "netint.h"

namespace SDSocket
{
  class Buffer
  {
    friend class Socket;

  protected:
    // Returns the pointer to the data. (write access)
    virtual char* pointer() throw() = 0;
    // Returns the pointer to the data. (read access)
    virtual const char* pointer() const throw() = 0;
    // Returns the buffer size in bytes.
    virtual size_t size() const throw() = 0;
    // Used by recv() and recvfrom() to prepare buffer before putting data into buffer.
    virtual void prepare() throw() = 0;
    // Called by recv() and recvfrom() when data is transfered.
    virtual void update(size_t bytes) throw() = 0;

  public:
    virtual ~Buffer() throw() {}
  };
}

#endif
