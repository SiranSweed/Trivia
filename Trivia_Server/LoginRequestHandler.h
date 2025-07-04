#pragma once
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include <exception>
#include "MsgCodes.h"
#include "IRequestHandler.h"
#include "Responses.h"
#include "RequestHandlerFactory.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory* handlerFactory);
	virtual ~LoginRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& reqInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& reqInfo) override;

private:
	// Auxiliary functions
	RequestResult login(const RequestInfo& reqInfo);
	RequestResult signup(const RequestInfo& reqInfo);
	RequestResult logout(const RequestInfo& reqInfo);

	// Class filed
	RequestHandlerFactory* m_handlerFactory;
};
