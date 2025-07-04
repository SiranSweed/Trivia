#include "RequestHandlerFactory.h"


RequestHandlerFactory* RequestHandlerFactory::_factory = nullptr;


RequestHandlerFactory* RequestHandlerFactory::getInstance(IDatabase* database)
{
    if (_factory == nullptr)
    {
        _factory = new RequestHandlerFactory(database);
    }
    return _factory;
}

/*
This function creates a new pointer to instance of a LoginRequestHandler.
Inputs: None.
Output: The new LoginRequestHandler created.
*/
IRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
	return new LoginRequestHandler(this);
}

/*
This function returns the login manager field
Inputs: None
Output: the login manager
*/
LoginManager& RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}

/*
This function returns the statistics manager field
Inputs: None
Output: the statistics manager
*/
StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
    return this->m_StatisticsManager;
}

/*
This function returns the room manager field
Inputs: None
Output: the room manager
*/
RoomManager& RequestHandlerFactory::getRoomManager()
{
    return this->m_roomManager;
}

IRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(LoggedUser user, Room& room)
{
    return new RoomAdminRequestHandler(*this, user, room);
}

IRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(LoggedUser user, Room& room)
{
    return new RoomMemberRequestHandler(*this, user, room);
}

/*
This function creates a new pointer to instance of a MenuRequestHandler.
Inputs: None.
Output: The new MenuRequestHandler created.
*/
IRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return new MenuRequestHandler(*this, user);
}

/*
This is the class' CTOR - prepares a new RequestHandlerFactory object
by initializing the m_database and allocating the m_loginManager fields.
Inputs: A database to init with the m_database field.
*/
RequestHandlerFactory::RequestHandlerFactory(IDatabase* database) :
	m_database(database), m_loginManager(LoginManager(database)), m_roomManager(RoomManager()), m_StatisticsManager(StatisticsManager(database))
{
}
