#pragma comment (lib, "ws2_32.lib")
#include "Server.h"
#include "WSAInitializer.h"
#include <iostream>

using std::cerr;
using std::endl;

int main()
{
	try
	{
		WSAInitializer wsa_init;
		Server triviaServer;
		triviaServer.run();
	}
	catch (const std::exception& e)
	{
		cerr << "Exception was thrown in function: " << e.what() << endl;
	}
	catch (...)
	{
		cerr << "Unknow exception was thrown!" << endl;
	}
}