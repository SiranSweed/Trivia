#pragma once

#include "IDatabase.h"
#include "SqliteDatabase.h"
#include "Communicator.h"
#include "RequestHandlerFactory.h"

class Server
{
public:
	Server();
	~Server();
	void run();

private:
	Communicator* m_communicator;
	IDatabase* m_database;
	RequestHandlerFactory* m_handlerFactory;
};
