#pragma once

#include "IRequestHandler.h"
#include "Responses.h"
#include "Requests.h"
#include "JsonRequestPacketDeserializer.h"
#include "RequestHandlerFactory.h"
#include "JsonResponsePacketSerializer.h"

class RequestHandlerFactory;
class Room;
class LoggedUser;

class RoomAdminRequestHandler : public IRequestHandler
{
public:
	RoomAdminRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user, Room& room);
	virtual ~RoomAdminRequestHandler() override = default;

	virtual bool isRequestRelevant(const RequestInfo& reqInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& reqInfo) override;

private:

	// Auxiliary functions
	RequestResult startGame(const RequestInfo& reqInfo);
	RequestResult closeRoom(const RequestInfo& reqInfo);
	RequestResult getRoomState(const RequestInfo& reqInfo);

	LoggedUser getUser() const;

	Room& m_room;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
};
