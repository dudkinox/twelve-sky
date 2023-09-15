/****************************************************************************

   CTSD : Common Transaction Service 

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: cts_signal.h,v 1.2 2007/08/03 05:24:03 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/
#ifndef __SIGNAL_H__
#define __SIGNAL_H__

enum e_process_type {
	PARENT,
	GUARD,
	CHILD
};

extern int init_signals(int flag);

#endif
