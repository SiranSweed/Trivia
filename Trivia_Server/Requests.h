#pragma once

#include <string>

using std::string;

struct LoginRequest
{
	string m_username;
	string m_password;
};

struct SignupRequest
{
	string m_username;
	string m_password;
	string m_email;
};

struct GetPlayersInRoomRequest
{
	unsigned int _roomId;
};

struct JoinRoomRequest
{
	unsigned int _roomId;
};

struct CreateRoomRequest
{
	string _roomName;
	unsigned int maxUsers;
	unsigned int _questionCount;
	unsigned int _answerTimeout;
};
