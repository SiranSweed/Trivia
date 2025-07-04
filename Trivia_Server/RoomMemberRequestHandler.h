#pragma once

#include "IRequestHandler.h"
#include "Requests.h"
#include "Responses.h"
#include "RequestHandlerFactory.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

class RequestHandlerFactory;
class Room;
class LoggedUser;

// We can do that the AdminRequestHandler inherince from RoomMemberRequestHandler but with more function (like the startGame)

class RoomMemberRequestHandler : public IRequestHandler
{
public:

	RoomMemberRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user, Room& room);
	virtual ~RoomMemberRequestHandler() override = default;

	virtual bool isRequestRelevant(const RequestInfo& reqInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& reqInfo) override;

	LoggedUser getUser() const;

private:

	RequestResult leaveRoom(const RequestInfo& reqInfo);
	RequestResult getRoomState(const RequestInfo& reqInfo);
	RequestResult startGame(const RequestInfo& reqInfo);

	Room& m_room;
	LoggedUser m_user;
	RequestHandlerFactory& m_handlerFactory;
};
