#pragma once

#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include <vector>

#define MEANINGLESS_ID 0

class RequestHandlerFactory;

class MenuRequestHandler : public IRequestHandler
{
public:
	MenuRequestHandler(RequestHandlerFactory& handlerFactory, const LoggedUser& user);
	virtual ~MenuRequestHandler() override = default;

	virtual bool isRequestRelevant(const RequestInfo& reqInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& reqInfo) override;

private:

	// Auxiliary functions
	RequestResult logout(const RequestInfo& reqInfo);
	RequestResult getRooms(const RequestInfo& reqInfo);
	RequestResult getPlayersInRoomRequest(const RequestInfo& reqInfo);
	RequestResult getPersonalStats(const RequestInfo& reqInfo);
	RequestResult getHighScore(const RequestInfo& reqInfo);
	RequestResult joinRoom(const RequestInfo& reqInfo);
	RequestResult createRoom(const RequestInfo& reqInfo);

	RequestHandlerFactory& m_handlerFactory;
	LoggedUser m_user;
};
