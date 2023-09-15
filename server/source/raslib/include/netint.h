/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: netint.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __NETINT_H__
#define __NETINT_H__

#include <netinet/in.h>

namespace SDSocket
{
  class NetInt32
  {
  public:
    NetInt32(uint32_t val = 0) throw()
    {
      value = htonl(val);
    }

    uint32_t operator=(uint32_t val) throw()
    {
      value = htonl(val);

      return val;
    }

    operator uint32_t() const throw()
    {
      return ntohl(value);
    }

  private:
    uint32_t value;
  };

  class NetInt16
  {
  public:
    NetInt16(uint16_t val = 0) throw()
    {
      value = htons(val);
    }

    uint16_t operator=(uint16_t val) throw()
    {
      value = htons(val);

      return val;
    }

    operator uint16_t() const throw()
    {
      return ntohs(value);
    }

  private:
    uint16_t value;
  };
}
#endif
