/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: rconfig.h,v 1.3 2007/05/13 12:46:10 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <iostream>
#include <string>
#include <list>

#include "exception.h"

using namespace std;		
namespace SDSocket {
enum 
{ 
	DEFAULT_MAX_KEYSIZE = 64, 
	DEFAULT_MAX_VALSIZE = 512,
	DEFAULT_MAX_CONFIG_FILE_PATH = 4096
};

typedef struct _Config_t
{
	char Key[DEFAULT_MAX_KEYSIZE];
	char Value[DEFAULT_MAX_VALSIZE];
} Config_t;

class Config
{
public:
	Config(const char *pszFileName) throw();
	Config() throw() {}
	virtual ~Config();
	void SetConfigFile(const char *pszFileName) throw();
	void Register(const char *pKey);
	void Deregister(const char *pKey);
	virtual int  LoadConfig();
	int  CheckDup(const char *pKey);
	int  SetValue(const char *pKey, const char *pValue);
	virtual int  GetValue(const char *pKey, char *pValue);		
	void  DumpAll(void); 
protected:
	int GetKeyData(const char *pSrc, char *pKey, char *pData);
	char m_pFileName[DEFAULT_MAX_CONFIG_FILE_PATH];		
	std::list<Config_t> m_ConfigList; 

	class CompareKey
	{
	public:
		CompareKey(const char *pKey) { sprintf(m_pKey, "%s", pKey); }
		bool operator() (const Config_t &pIC) {
			if(!strcmp(pIC.Key, m_pKey)) return 1;
			return 0;
		}
		char m_pKey[DEFAULT_MAX_KEYSIZE];
	};
};
}
#endif
