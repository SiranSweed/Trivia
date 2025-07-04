#pragma once

#include "json.hpp"
#include "Responses.h"
#include "MsgCodes.h"
#include <vector>
#include <string>
#include <sstream>

using std::vector;
using std::string;
using json = nlohmann::json;

#define BYTE_MASK 0xFF
#define MSG_LEN 4


class JsonResponsePacketSerializer
{
public:

	/* Static functions that serializes responses to send them to the client */
	static vector<unsigned char> serializeResponse(const LoginResponse response);
	static vector<unsigned char> serializeResponse(const SignUpResponse response);
	static vector<unsigned char> serializeResponse(const ErrorResponse response);
	static vector<unsigned char> serializeResponse(const LogoutResponse response);
	static vector<unsigned char> serializeResponse(const GetRoomsResponse response);
	static vector<unsigned char> serializeResponse(const GetPlayersInRoomResponse response);
	static vector<unsigned char> serializeResponse(const JoinRoomResponse response);
	static vector<unsigned char> serializeResponse(const CreateRoomResponse response);
	static vector<unsigned char> serializeResponse(const getHightScoreResponse response);
	static vector<unsigned char> serializeResponse(const getPersonalStatsResponse response);
	static vector<unsigned char> serializeResponse(const CloseRoomResponse response);
	static vector<unsigned char> serializeResponse(const StartGameResponse response);
	static vector<unsigned char> serializeResponse(const GetRoomStateResponse response);
	static vector<unsigned char> serializeResponse(const LeaveRoomResponse response);
};
