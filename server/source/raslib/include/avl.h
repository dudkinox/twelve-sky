/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: avl.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/
 
#ifndef __AVL_H__
#define __AVL_H__
 
#include "exception.h"

typedef struct __avl
{
	struct __avl *pLeft;
	struct __avl *pRight;
	signed char balance;
} avl;

typedef struct _avl_tree
{
	avl *pRoot;
	int (*func)(void *pEa, void *pEb);
} avl_tree;
	
class AVLTree 
{
public:
	int Insert(avl *pElement) throw ( Exception );
	int Remove(avl *pElement) throw (;
	int RemoveRoot() throw ();
	int Range(avl *pElement1, avl *pElement2, int(*iter)(avl *pElement)) throw();
	int Search(avl *pElement, int(*iter)(avl *pElement) throw();

pro
