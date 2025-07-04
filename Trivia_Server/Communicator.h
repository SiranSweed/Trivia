#pragma once

#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestInfo&Result.hpp"
#include <map>
#include <mutex>
#include <WinSock2.h>
#include <Windows.h>

using std::map;
using std::mutex;

static unsigned short LISTENING_PORT = 4250;

class Communicator 
{
public:

	/*This function binds and listens on the defined port(with the bindAndListen()
	auxiliary function), and then accepts nonstop connecting clients. */
	void startHandleRequests();

	// Did the Bonus Singleton implementation for the Communicator object because it's easy like that
	static Communicator* getInstance(RequestHandlerFactory* handlerFactory);

private:
	/*Private ctor and dtor to enfore singleton */
	//This is the class - Ctor - which prepares a new Communicator object by opening a TCP listening Socket
	Communicator(RequestHandlerFactory* handlerFactory);

	//This is the class - Dtor - closes the listening socket
	~Communicator();

	// Delete Copy Constructor and Assignment Operator
	Communicator(const Communicator&) = delete;
	Communicator& operator=(const Communicator&) = delete;

	// Private methods
	void bindAndListen();
	void accpetClient();
	void handleNewClient(const SOCKET clientSocket);
	//std::string getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags);

	SOCKET m_serverSocket;
	map<SOCKET, IRequestHandler*> m_clients;
	mutex m_ClientsMapMtx;

	RequestHandlerFactory* m_handleFactory;

	static Communicator* _communicator;
};

