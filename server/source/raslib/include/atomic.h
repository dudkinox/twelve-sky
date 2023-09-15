/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: atomic.h,v 1.2 2007/04/23 02:08:47 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __ATOMIC_H__
#define __ATOMIC_H__

#include <sys/types.h>

#if defined(__i586__) || defined (__i686__)
static __inline__ void atomic_inc_uint32(unsigned long* num_ptr)
{
	__asm__ __volatile__(
			"lock incl (%0);"               
			: /* no output */
			: "r" (num_ptr)
			: "cc", "memory"
			);
}

static __inline__ void atomic_dec_uint32(unsigned long* num_ptr)
{
	__asm__ __volatile__(
			"lock decl (%0);"                
			: /* no output */
			: "r" (num_ptr)
			: "cc", "memory"
			);
}

static __inline__ unsigned long atomic_inc_ret_uint32(unsigned long* num_ptr)
{
	volatile unsigned long ret_val;
	__asm__ __volatile__(
			"lock xaddl %0, (%1);"           
			"     inc   %0;"                 
			: "=r" (ret_val)
			: "r" (num_ptr), "0" (0x1)
			: "cc", "memory"
			);
	return ret_val;
}

static __inline__ unsigned long atomic_dec_ret_uint32(unsigned long* num_ptr)
{   
	volatile unsigned long ret_val;
	__asm__ __volatile__(
			"lock xaddl %0, (%1);"           
			"     dec   %0;"                 
			: "=r" (ret_val)
			: "r" (num_ptr), "0" (-1)
			: "cc", "memory"
			);
	return ret_val;
}

static __inline__ int atomic_dec_and_test_uint32(unsigned long* num_ptr)
{
	unsigned char c;

	__asm__ __volatile__(
			"lock decl (%0); sete %1"
			: "=qm" (c)
			: "r" (num_ptr)
			: "cc", "memory");
	return c;
}

#endif
#if defined(__asm64__) || defined(__x86_64__)
static __inline__ void atomic_inc_uint32(unsigned long* num_ptr)
{
	__asm__ __volatile__(
			"lock incl (%%rax);"             
			: /* no output */
			: "a" (num_ptr)
			: "cc", "memory"
			);
}

static __inline__ void atomic_dec_uint32(unsigned long* num_ptr)
{
	__asm__ __volatile__(
			"lock decl (%%rax);"             
			: /* no output */
			: "a" (num_ptr)
			: "cc", "memory"
			);
}

static __inline__ unsigned long atomic_inc_ret_uint32(unsigned long* num_ptr)
{
	volatile unsigned long ret_val;
	__asm__ __volatile__(
			"lock xaddl %%ebx, (%%rax);"     
			"     incl  %%ebx;"              
			: "=b" (ret_val)
			: "a" (num_ptr), "b" (0x1)
			: "cc", "memory"
			);
	return ret_val;
}

static __inline__ unsigned long atomic_dec_ret_uint32(unsigned long* num_ptr)
{   
	volatile unsigned long ret_val;
	__asm__ __volatile__(
			"lock xaddl %%ebx, (%%rax);"     
			"     decl  %%ebx;"             
			: "=b" (ret_val)
			: "a" (num_ptr), "b" (-1)
			: "cc", "memory"
			);
	return ret_val;
}

static __inline__ int atomic_dec_and_test_uint32(unsigned long* num_ptr)
{
	unsigned char c;

	__asm__ __volatile__(
			"lock decl (%%rax); sete %1"
			: "=qm" (c)
			: "a" (num_ptr)
			: "cc", "memory"
			);
	return c;
}


#endif
static __inline__ bool atomic_read_try_lock(unsigned long* num_ptr)
{
	atomic_dec_uint32(num_ptr);
	if(*num_ptr >= 0) {
		return false;
	} else {
		atomic_inc_uint32(num_ptr);
	} 
	return true;
}

static __inline__ bool atomic_write_try_lock(unsigned long* num_ptr)
{
	if(atomic_dec_and_test_uint32(num_ptr)) {
		return false;
	} else {
		atomic_inc_uint32(num_ptr);
	}
	return true;
}

static __inline__ bool _M_spin_is_locked(unsigned long* num_ptr)
{
	return *(num_ptr) <= 0;
}

static __inline__ void _M_pause_cpu(void) 
{
	__asm__ __volatile__("rep;nop": : :"memory");
}

static __inline__ void spin_unlock_wait(unsigned long* num_ptr)
{
	while (_M_spin_is_locked(num_ptr)) 
		_M_pause_cpu();
}

#endif
