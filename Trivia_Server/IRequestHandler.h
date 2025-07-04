#pragma once

#include "RequestInfo&Result.hpp"
#include "MsgCodes.h"

class IRequestHandler
{
public:
	IRequestHandler() = default;
	virtual ~IRequestHandler() = default;

	virtual bool isRequestRelevant(const RequestInfo& reqInfo) = 0;
	virtual RequestResult handleRequest(const RequestInfo& reqInfo) = 0;
};
