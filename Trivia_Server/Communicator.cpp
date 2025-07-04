#include "Communicator.h"
#include "CommunicatorHelper.h"
#include "MsgCodes.h"
#include <iostream>
#include <string>
#include <thread>
#include <exception>

using std::cout;
using std::endl;
using std::exception;
using std::thread;
using std::string;
using std::unique_lock;
using std::lock_guard;

Communicator* Communicator::_communicator;

/*
This is the class - Ctor - which prepares a new Communicator object by opening a TCP listening Socket
*/
Communicator::Communicator(RequestHandlerFactory* handlerFactory) : m_handleFactory(handlerFactory)
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
	{
		throw exception(__FUNCTION__ " - socket");
	}
}

/*
This is the class - Dtor - closes the listening socket
*/
Communicator::~Communicator()
{
	try
	{
		closesocket(this->m_serverSocket);
	}
	catch (...) {}
}

/*
This function binds and listens on the defined port (with the bindAndListen()
auxiliary function), and then accepts nonstop connecting clients.
Inputs: None.
Output: None.
*/
void Communicator::startHandleRequests()
{
	this->bindAndListen();

	while (true)
	{
		cout << "Waiting for client connection request." << endl;

		// After the handleNewClient thread in this func is detached, 
		// This infinite loop will continue to wait for another connection
		this->accpetClient();
	}
}

Communicator* Communicator::getInstance(RequestHandlerFactory* handlerFactory)
{
	if (_communicator == nullptr)
	{
		_communicator = new Communicator(handlerFactory);
	}
	return _communicator;
}

/*
This function binds and starts listening on the port defined as a constant in the header file.
Inputs: None.
Output: None.
*/
void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(LISTENING_PORT);	// The port that server will listen on
	sa.sin_family = AF_INET;				// Must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;	    // When there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
	{
		throw exception(__FUNCTION__ " - bind");
	}
	// Start listening for incoming requests of clients
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw exception(__FUNCTION__ " - listen");
	}
	cout << "Listening on port " << LISTENING_PORT << endl;
}

/*
This function accepts a new client that connects,
and communicates with him in a new independent thread.
Inputs: None.
Output: None.
*/
void Communicator::accpetClient()
{
	// This accepts the client and creates a specific socket from the server to this client
	// The process won't continue until a client connects to the server
	SOCKET clientSocket = accept(this->m_serverSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET)
	{
		throw exception(__FUNCTION__);
	}
	cout << "Client accepted. Server and client can speak" << endl;

	// Locks the clients mutex here, before creating the thread, so that the pair 
	// won't be removed from the m_clients map before it has even been inserted
	lock_guard<mutex> clientsLck(this->m_ClientsMapMtx);

	// Add the new client's socket to the m_clients map
	this->m_clients[clientSocket] = this->m_handleFactory->createLoginRequestHandler();

	thread handleNewClient(&Communicator::handleNewClient, this, clientSocket);
	handleNewClient.detach();

	// Add the new client's socket to the m_clients map
	//this->m_clients[clientSocket] = this->m_handleFactory.createLoginRequestHandler();
}

/*
This function communicates with the client that was accepted and send hello , while waiting for user's response
*/
void Communicator::handleNewClient(const SOCKET clientSocket)
{
	int currReqCode = 0, currDataSize = 0, flags = 0;
	vector<byte> reqBuffer;

	RequestInfo clientRequest;
	RequestResult reqResult;

	do
	{
		try
		{
			currReqCode = CommunicatorHelper::getMsgCodeFromSock(clientSocket);
		}
		catch (std::exception& e)
		{
			break;
		}

		currDataSize = CommunicatorHelper::getDataSizeFromSock(clientSocket);
		if (currDataSize != 0)
		{
			reqBuffer = CommunicatorHelper::getContentFromSock(clientSocket, currDataSize);
		}

		clientRequest._id = currReqCode;
		clientRequest._buffer = reqBuffer;

		unique_lock<mutex> clientsLck(this->m_ClientsMapMtx);
		// We use the isRequestRelevant in the handlerRequest function
		reqResult = this->m_clients[clientSocket]->handleRequest(clientRequest);

		// Move to another state in the state machine
		if (reqResult._newHandler != this->m_clients[clientSocket])
		{
			delete(this->m_clients[clientSocket]);
			this->m_clients[clientSocket] = reqResult._newHandler;
		}

		clientsLck.unlock();

		string reqResultStr(reqResult._Buffer.begin(), reqResult._Buffer.end());
		if (send(clientSocket, reqResultStr.c_str(), reqResultStr.size(), flags) == INVALID_SOCKET)
		{
			throw exception("Error while sending message to client");
		}

	} while (reqResult._Buffer[0] != ResponsesCode::LOGOUT_RESP_CODE);

	// After getting permission to access the map, free all the allocated resources
	unique_lock<mutex> clientsLck(this->m_ClientsMapMtx);
	this->m_clients.erase(clientSocket);
	cout << "Client " << clientSocket << " disconnected" << endl << endl;
	closesocket(clientSocket);
}
