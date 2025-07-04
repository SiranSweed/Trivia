#pragma once

#include <ctime>
#include <vector>
#include "IRequestHandler.h"

using std::time_t;
using std::vector;

class IRequestHandler; // Avoid circular reference errors

struct RequestInfo
{
	unsigned char _id;
	time_t _receivalTime;
	vector<unsigned char> _buffer;

	// Ctor for the struct
	RequestInfo() : _id(0)
	{
		// Initializes the time field with the current each time an instance is created
		std::time(&(this->_receivalTime));
	}
};

struct RequestResult
{
	IRequestHandler* _newHandler;
	vector<unsigned char> _Buffer;
};
