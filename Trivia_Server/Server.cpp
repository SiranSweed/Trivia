#include "Server.h"
#include <iostream>
#include <exception>
#include <string>
#include <thread>

using std::cout;
using std::cin;
using std::endl;
using std::thread;
using std::string;
using std::exception;

/*
This is the class - Ctor - creates a new server object, with the neccesairly params
*/
Server::Server()
{
	m_database = new SqliteDatabase();
	m_handlerFactory = RequestHandlerFactory::getInstance(m_database);
	m_communicator = Communicator::getInstance(m_handlerFactory);
	this->m_database;
	int a = 0;
}

/*
This the class - Dtor - for now no use
*/
Server::~Server()
{
	if (!m_database->close())
	{
		std::cerr << "Failed closing the DB" << endl;
	}
	delete(this->m_database);
}

/*
This function serves multiple clients simultaneously
until the string "EXIT" is received from the terminal.
Inputs: None.
Output: None.
*/
void Server::run()
{
	string theInput = "Not Exit";

	thread t_connector(&Communicator::startHandleRequests, (this->m_communicator));
	t_connector.detach();

	while (theInput != "EXIT")
	{
		std::cin >> theInput;
	}
}
