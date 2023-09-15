/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: raslib.h,v 1.2 2007/05/13 12:46:10 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __RASLIB_H__
#define __RASLIB_H__

#if (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7))
# define PUNUSED(x) __attribute__ ((unused)) x
# define UNUSED     __attribute__ ((unused))
#else
# define PUNUSED(x) x
# define UNUSED
#endif

#define TRACE()  fprintf(stderr, "TRACE> FILE :: %s, FUNCTION :: %s, POS :: %d\n", __FILE__, __FUNCTION__ , __LINE__)
#define DEBUG0(parameter) fprintf(stderr, parameter)
#define DEBUG1(parameter, value) fprintf(stderr, parameter, value)
#define DEBUG2(parameter, value1, value2) fprintf(stderr, parameter, value1, value2)
#define DEBUG3(parameter, value1, value2, value3) fprintf(stderr, parameter, value1, value2, value3)

#endif

