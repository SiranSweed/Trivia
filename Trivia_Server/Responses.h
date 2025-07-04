#pragma once

#include <string>
#include <vector>
#include "RoomData.h"

using std::string;
using std::vector;

struct LoginResponse
{
	unsigned int _status;
};

struct SignUpResponse
{
	unsigned int _status;
};

struct LogoutResponse
{
	unsigned int _status;
};

struct GetRoomsResponse
{
	unsigned int _status;
	std::vector<RoomData> _rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<string> _players;
};

struct getHightScoreResponse
{
	unsigned int _status;
	std::vector<string> _statistics;
};

struct getPersonalStatsResponse
{
	unsigned int _status;
	std::vector<string> _statistics;
};

struct JoinRoomResponse
{
	unsigned int _status;
};

struct CreateRoomResponse
{
	unsigned int _status;
};

struct ErrorResponse
{
	string _message;
};

struct CloseRoomResponse
{
	unsigned int _status;
};

struct StartGameResponse
{
	unsigned int _status;
};

struct GetRoomStateResponse
{
	unsigned int _status;
	bool _hasGameBegun;
	bool _isActive;
	vector<string> _players;
	unsigned int _questionCount;
	unsigned int _answerTimeout;
};

struct LeaveRoomResponse
{
	unsigned int _status;
};


