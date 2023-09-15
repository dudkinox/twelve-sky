#include <iostream>
#include <string>

#include "log.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "tcpconnection.h"

#include <sys/types.h>
#include <sys/signal.h>

using namespace std;
using namespace SDLog;
using namespace SDSocket;


// Simple Client
/*
int main(void)
{
	try
	{
		string StrRcvd;
		SDSocket::TCPClient Client;
		SDSocket::Address ClientAddress("127.0.0.1", 39000);
		
		cout << "Connect to " << ClientAddress.getIpAddr()
				 << " Port : " << ClientAddress.getPort() << endl;
		Client.connect(ClientAddress);
		Client >> StrRcvd;
		fprintf(stdout, "> %s\n", StrRcvd.c_str());
		Client.disconnect();
	}
	catch (Exception &Error)
	{
		cerr << "Module : " << Error.getMethod() << endl;
		cerr << "Message : " << Error.getMessage() << endl;
		return -1;
	}
	return 1;
}
*/

int main(void)
{
	SDSocket::TCPServer Server;
	try
	{
		Server.start(13112, 10);
		SDSocket::Address serverAddress;
		serverAddress = Server.getAddress();
		//serverAddress.lookup();
		
		cout << "Server: " << serverAddress.getName() << endl;
		cout << "Wait on Port: " << serverAddress.getPort() << endl;
		
		while(1)
		{
			cout << "Waiting...." << endl;
			SDSocket::TCPConnection* connection;
			SDSocket::Address clientAddress;
			
			connection = Server.getConnection(clientAddress);
			try
			{
				clientAddress.lookup();
			}
			catch (Exception &Error)
			{
				cerr << "UNknown HostName" << endl;
			}
			
			string strText;
			

			*connection >> strText;
			
			cout << "Client: " << clientAddress.getIpAddr() << endl;
			cout << "Text: " << strText << strText.length() <<  endl;
			connection->disconnect();
			delete connection;
			
		}
	}
	catch (Exception &Error)
	{
		cerr << "Module : " << Error.getMethod() << endl;
		cerr << "Message : " << Error.getMessage() << endl;
		Server.stop();
	}
	return 1;
}

