/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: exception.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <errno.h>
#include <netdb.h>
#include <string>
#include <string.h>

#include <exception>

namespace SDSocket
{
  using std::string;
  using std::exception;
  
  class Exception : public exception
  {
  public:
    Exception(const string& _method, const string& _message, int _code) throw()
      : method(_method), message(_message), code(_code)
    {}
	Exception(const string& _method, const string& _message)
	  : method(_method), message(_message)
	{}
		
    virtual ~Exception() throw() {}
    
    const string& getMethod()  const throw() { return method;  }
    const string& getMessage() const throw() { return message; }
    int getCode()              const throw() { return code;    }

    virtual const char* what() const throw() { return getMessage().c_str(); }
    
  protected:
    string method;  // Method, which throwed the eception
    string message; // Errormessage
    int code;       // Errorcode
  };

  class LibCException : public Exception
  {
  public:
    LibCException(const string& _method) throw()
      : Exception(_method, strerror(errno), errno)
    {}
  };

  class HLibCException : public Exception
  {
  public:
    HLibCException(const string& _method) throw()
      : Exception(_method, hstrerror(h_errno), h_errno)
    {}
  };
}
#endif
