/****************************************************************************

  Ras library FreeBSD Version                                               

  CopyRight(C) 2003 Sean Kim All Rights reserved.           
Author : Sean Kim (sean@0x31.com)                                      

$Id: address.h,v 1.2 2007/06/21 10:53:56 sean Exp $              

Redistribution and use in source and binary forms, with or with out      
modification, are not permitted without author's permission. 

 ****************************************************************************/

#ifndef __ADDRESS_H__
#define __ADDRESS_H__

#include <string>
#include <vector>
#include <netinet/in.h>

#include "exception.h"

using namespace std;
namespace SDSocket
{
	class Address
	{
	public:
		// Errorcodes (Without LibC-errorcodes)
		enum ErrorCode
		{
			NO_IP_CODE     = 0x0101, ///< No valid IP address
			OFF_RANGE_CODE = 0x0102  ///< Index out of valid range
		};

		// Map CPPSocket Port type to system-dependant port type
		typedef in_port_t Port;

		// Can be used when no IP-address should be specified
		static const string ANY_IP;

		explicit Address(const string& ip = ANY_IP, Port port = 0, bool lookup = false) throw (Exception);
		Address(const sockaddr_in& address) throw();
		Address(int NotUsed) throw() {}
		operator sockaddr_in() const throw (Exception);
		void operator=(const sockaddr_in& address) throw();
		void operator=(const Address& address) throw();
		void lookup() throw (Exception);
		const string& getName() const throw();
		int getNumAliases() const throw();
		const string& getAliasName(size_t index) const throw (Exception);
		int getNumIPs() const throw();
		string getIpAddr() const throw();
		string getIPString(size_t index = 0) const throw (Exception);
		Port getPort() const throw();
		void SetAddress(const string& ip = ANY_IP, Port Port = 0, bool lookup = false) throw (Exception);
		void ResetAddress();

	private:
		typedef std::vector<string> StringVector;
		typedef std::vector<in_addr> IPVector;

		// Name of the Machine
		string name;
		// An Array of alias names
		StringVector aliases;
		// An Array of ip-addresses in netbyteorder
		IPVector ipAddresses;
		// The portnumber in netbyteorder
		Port port;
		void lookup(const string& ip) throw (Exception);
	};
}

#endif
