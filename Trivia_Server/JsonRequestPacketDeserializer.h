#pragma once
#include <vector>
#include <string>
#include "json.hpp"
#include "Requests.h"
#include "MsgCodes.h"

using std::vector;
using std::string;
using json = nlohmann::json;


class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const vector<unsigned char>& buffer);
	static SignupRequest deserializeSignupRequest(const vector<unsigned char>& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const vector<unsigned char>& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const vector<unsigned char>& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const vector<unsigned char>& buffer);
};

