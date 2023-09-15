/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: config.cpp,v 1.4 2007/04/30 00:50:55 sean Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdio>
#include <string.h>
#include "util.h"
#include "rconfig.h"
#include "bytebuffer.h"

using namespace std;
using namespace SDSocket;

Config::Config(const char *pszFileName) throw()
{
	::snprintf(m_pFileName, DEFAULT_MAX_CONFIG_FILE_PATH, pszFileName);
}

Config::~Config() 
{
	/*
	std::list<Config_t>::iterator i;
	
	for(i = m_ConfigList.begin(); i != m_ConfigList.end(); ++i)
	{
		m_ConfigList.erase(i);
	}
	*/
}

void Config::SetConfigFile(const char *pszFileName) throw()
{
	::snprintf(m_pFileName, DEFAULT_MAX_CONFIG_FILE_PATH, pszFileName);
}

void Config::Register(const char *pKey) 
{
	Config_t RegKey;
		
	::memset((char *)RegKey.Key, 0, DEFAULT_MAX_KEYSIZE);
	::snprintf(RegKey.Key, DEFAULT_MAX_KEYSIZE, pKey);
	m_ConfigList.push_back(RegKey);
}

void Config::Deregister(const char *pKey)
{	
#ifdef __NEW_STL__
	std::list<Config_t>::iterator location = find_if(m_ConfigList.begin(), m_ConfigList.end(), CompareKey(pKey));
	m_ConfigList.erase(location);
#else
	std::list<Config_t>::iterator i;

	for(i = m_ConfigList.begin(); i != m_ConfigList.end(); ++i) {
		if(!::strcmp(pKey, i->Key)) m_ConfigList.erase(i);
	}
#endif
}

int Config::CheckDup(const char *pKey) 
{
	int fFound = 1;
	std::list<Config_t>::const_iterator i;
	
	for(i = m_ConfigList.begin(); i != m_ConfigList.end(); ++i) {
		if(!_strcmp_ac(i->Key, pKey)) fFound = 0;
	}				
	
  return fFound;
}

void Config::DumpAll() 
{
	std::list<Config_t>::const_iterator i;
	
	for(i = m_ConfigList.begin(); i != m_ConfigList.end(); ++i) {
		cerr << "i->Key = " << i->Key << ", i->Value = " << i->Value  << endl;
	}				
}

int Config::SetValue(const char *pKey, const char *pValue)
{
	int fFound = 0;
	std::list<Config_t>::const_iterator i;
	
	for(i = m_ConfigList.begin(); i != m_ConfigList.end(); ++i) {
		if(!_strcmp_ac(i->Key, pKey)) {
			fFound = 1;
			::memset((char *)i->Value, 0, DEFAULT_MAX_VALSIZE);
			::snprintf((char *)i->Value, DEFAULT_MAX_VALSIZE, pValue);
		}
	}
	return fFound;
}

int Config::GetValue(const char *pKey, char *pValue) 
{
	int fFound = 0;
	std::list<Config_t>::const_iterator i;
	
	if(pValue == NULL) return -1;
	for(i = m_ConfigList.begin(); i != m_ConfigList.end(); ++i) {
		if(!_strcmp_ac(i->Key, pKey)) {
			::snprintf(pValue, DEFAULT_MAX_VALSIZE, i->Value);
			fFound = 1;
		}
	}
	return fFound;
}

int Config::GetKeyData(const char *pSrc, char *pKey, char *pData)
{
	int c_count = 0;
	bool is_found = false;

	while(*pSrc != '\0') {
		if(*pSrc == '=') {
			is_found = true;
			pKey[c_count] = '\0';
			pSrc++;
			break;
		}
		pKey[c_count++] = *pSrc++;
	}

	if(!is_found) {
		return -1;
	}

	c_count = 0;

	while(*pSrc != '\0') {
		if(*pSrc == '=') {
			pData[c_count] = '\0';
			break;
		}
		pData[c_count++] = *pSrc++;
	}

	return 1;
}

#define MAX_SIZE_OF_LINE_BUFFER 1024
int Config::LoadConfig() 
{
	char line_buffer[MAX_SIZE_OF_LINE_BUFFER];
	char tmpKey[DEFAULT_MAX_KEYSIZE];
	char tmpVal[DEFAULT_MAX_VALSIZE];
	
	ifstream in(m_pFileName);
	if(!in.is_open()) return -1;
	
	while(in.getline(line_buffer, MAX_SIZE_OF_LINE_BUFFER)) {
		if((line_buffer[0] == '#') || (line_buffer[0] == '\0')) {
			continue;
		}
		if(!GetKeyData(line_buffer, tmpKey, tmpVal)) {
			continue;
		}
		_trim(tmpKey, NULL);
		_ltrim(tmpVal, NULL);
		if(CheckDup(tmpKey)) {
			Register(tmpKey);
		}
		SetValue(tmpKey, tmpVal);
		::memset(line_buffer, 0, MAX_SIZE_OF_LINE_BUFFER);
		::memset(tmpKey, 0, DEFAULT_MAX_KEYSIZE);
		::memset(tmpVal, 0, DEFAULT_MAX_VALSIZE);
	}
	in.close();
	return 1;
}
