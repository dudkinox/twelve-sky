/****************************************************************************

   Ras library FreeBSD Version

   CopyRight(C) 2003 Sean Kim All Rights reserved.
   Author : Sean Kim (sean@0x31.com)

   $Id: blowfish.h,v 1.1 2007/03/21 06:14:07 miracle Exp $

   Redistribution and use in source and binary forms, with or with out
   modification, are not permitted without author's permission.

 ****************************************************************************/

#ifndef __BLOWFISH_H__
#define __BLOWFISH_H__

#define MAXKEYBYTES			56		// 448 bits max
#define NPASS           16		// SBox passes

#define DWORD  		unsigned long
#define WORD  		unsigned short
#define BYTE  		unsigned char

class CBlowFish
{
private:
	DWORD 		* PArray ;
	DWORD		(* SBoxes)[256];
	void 		Blowfish_encipher (DWORD *xl, DWORD *xr) ;
	void 		Blowfish_decipher (DWORD *xl, DWORD *xr) ;

public:
			CBlowFish () ;
			~CBlowFish () ;
	void 		Initialize (BYTE key[], int keybytes) ;
	DWORD		GetOutputLength (DWORD lInputLong) ;
	DWORD		Encode (BYTE * pInput, BYTE * pOutput, DWORD lSize) ;
	void		Decode (BYTE * pInput, BYTE * pOutput, DWORD lSize) ;

} ;

// choose a byte order for your hardware
#define ORDER_DCBA	// chosing Intel in this case

#ifdef ORDER_DCBA  	// DCBA - little endian - intel
	union aword {
	  DWORD dword;
	  BYTE byte [4];
	  struct {
	    unsigned int byte3:8;
	    unsigned int byte2:8;
	    unsigned int byte1:8;
	    unsigned int byte0:8;
	  } w;
	};
#endif

#ifdef ORDER_ABCD  	// ABCD - big endian - motorola
	union aword {
	  DWORD dword;
	  BYTE byte [4];
	  struct {
	    unsigned int byte0:8;
	    unsigned int byte1:8;
	    unsigned int byte2:8;
	    unsigned int byte3:8;
	  } w;
	};
#endif

#ifdef ORDER_BADC  	// BADC - vax
	union aword {
	  DWORD dword;
	  BYTE byte [4];
	  struct {
	    unsigned int byte1:8;
	    unsigned int byte0:8;
	    unsigned int byte3:8;
	    unsigned int byte2:8;
	  } w;
};
#endif

extern void Char2Hex(const unsigned char ch, char* szHex);
extern void Hex2Char(const char* szHex, unsigned char& rch);
extern void CharStr2HexStr(const unsigned char* pucCharStr, char* pszHexStr, int iSize);
extern void HexStr2CharStr(const char* pszHexStr, unsigned char* pucCharStr, int iSize);
#endif
