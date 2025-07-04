#pragma once

#include "IRequestHandler.h"
#include "LoginManager.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "IDatabase.h"
#include "SqliteDatabase.h"


class RequestHandlerFactory
{
public:

	// Delete Copy Constructor and Assignment Operator
	RequestHandlerFactory(const RequestHandlerFactory&) = delete;
	RequestHandlerFactory& operator=(const RequestHandlerFactory&) = delete;

	// Bonus Singletion!!
	static RequestHandlerFactory* getInstance(IDatabase* database = nullptr);

	IRequestHandler* createLoginRequestHandler();
	LoginManager& getLoginManager();

	IRequestHandler* createMenuRequestHandler(const LoggedUser& user);
	StatisticsManager& getStatisticsManager();
	RoomManager& getRoomManager();

	IRequestHandler* createRoomAdminRequestHandler(LoggedUser user, Room& room);
	IRequestHandler* createRoomMemberRequestHandler(LoggedUser user, Room& room);


private:
	// Private Ctor and Dtor
	RequestHandlerFactory(IDatabase* database);
	~RequestHandlerFactory() = default;

	static RequestHandlerFactory* _factory;

	// Fields
	IDatabase* m_database;
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticsManager m_StatisticsManager;
};
