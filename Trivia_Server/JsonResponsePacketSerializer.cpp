#include "JsonResponsePacketSerializer.h"
#include <sstream>

using std::stringstream;

/*
This function serializes the login response to send back to a client, after receiving a valid login request.
Inputs: A LoginResponse struct variable that holds the status to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Convert login response to JSON format
    json data;
    data["status"] = response._status;

    // Serialize Json packet
    string jsonStr = data.dump();

    // Reserve enough memeory for the message
    buffer.reserve(1 + MSG_LEN + jsonStr.size());

    // Place the message code as the first byte
    buffer.push_back(LOGIN_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {
        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }

    return buffer;
}

/*
This function serializes the signup response to send back to a client, after receiving a valid signup request.
Inputs: A SignupResponse struct variable that holds the status to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignUpResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Convert login response to JSON format
    json data;
    data["status"] = response._status;

    // Serialize Json packet
    string jsonStr = data.dump();

    // Reserve enough memeory for the message
    buffer.reserve(1 + MSG_LEN + jsonStr.size());

    // Place the message code as the first byte
    buffer.push_back(SIGNUP_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {
        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }

    return buffer;
}

/*
This function serializes the error response to send back to a client, after receiving an invalid request.
Inputs: An Error Response struct variable that holds the status to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Convert login response to JSON format
    json data;
    data["status"] = response._message;

    // Serialize Json packet
    string jsonStr = data.dump();

    // Reserve enough memeory for the message
    buffer.reserve(1 + MSG_LEN + jsonStr.size());

    // Place the message code as the first byte
    buffer.push_back(ERROR_RESPONSE_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {
        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }

    return buffer;
}

/*
This function serializes the logout response to send back to a client, after receiving a valid logout request.
Inputs: A LogoutResponse struct variable that holds the status to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Coverts LogoutResponse to Json
    json data;
    data["status"] = response._status;

    // Serialize Json packet
    string jsonStr = data.dump();
    // Reserve enough memeory for the message
    buffer.reserve(1 + MSG_LEN + jsonStr.size());

    buffer.push_back(LOGOUT_RESP_CODE);
    
    // Size of the message
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {

        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }
    return buffer;
}

/*
This function serializes the getRooms response to send back to a client, after receiving a valid getRooms request.
Inputs: A GetRoomsResponse struct variable that holds the status and the rooms to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Create an array of JSON objects for rooms
    json roomsJson = json::array();
    for (const auto& room : response._rooms)
    {
        roomsJson.push_back({ {"id", room._id}, {"name", room._name} });
    }

    // Covert GetRoomsResponse to JSON format
    json data;
    data["status"] = response._status;
    data["rooms"] = roomsJson;

    // Serialize json format
    string jsonStr = data.dump();

    // Place the msg code as the first byte
    buffer.push_back(GET_ROOMS_RESP_CODE);

    // Size of the data
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {

        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }
    return buffer;
}

/*
This function serializes the getPlayersInRoom response to send back to a client, after receiving a valid getPlayersInRoom request.
Inputs: A GetPlayersInRoomResponse struct variable that holds the players to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Create an array of json for players
    json playersJson = json::array();
    for (const auto& player : response._players)
    {
        playersJson.push_back({ {"name", player} });
    }

    // Covert getPlayersInRoomResponse to JSON format
    json data;
    data["players"] = playersJson;

    // Serialize Json packet
    string jsonStr = data.dump();

    buffer.push_back(GET_PLAYERS_IN_ROOM_RESP_CODE);

    // Size of the data
    dataSize = jsonStr.size();

    // Covert the data size to binary and push it into the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {
        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the vector of bytes
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }
    return buffer;
}

/*
This function serializes the joinRoom response to send back to a client, after receiving a valid joinRoom request.
Inputs: A JoinRoomResponse struct variable that holds the status to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Covert JoinRoomResponse to json
    json data;
    data["status"] = response._status;

    // Serializes json packet
    string jsonStr = data.dump();

    buffer.push_back(JOIN_ROOM_RESP_CODE);

    // Size of the data
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {

        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }
    return buffer;
}

/*
This function serializes the createRoom response to send back to a client, after receiving a valid createRoom request.
Inputs: A CreateRoomResponse struct variable that holds the status to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Converts CreateRoomResponse to JSON format
    json data;
    data["status"] = response._status;

    // Serializes JSON packet
    string jsonStr = data.dump();

    // Places the msg code as the first byte
    buffer.push_back(CREATE_ROOM_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {

        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }
    return buffer;
}

/*
This function serializes the getHighScore response to send back to a client, after receiving a valid getHighScore request.
Inputs: A GetHighScoreResponse struct variable that holds the status and the statistics to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const getHightScoreResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Create an array of json for the highest scores
    json highestScoresJson = json::array();
    for (const auto& highScore : response._statistics)
    {
        // Covert each high score to a string using stringstream
        std::stringstream ss;
        ss << highScore;
        highestScoresJson.push_back(ss.str());
    }

    // Convert getHighScoreResponse to JSON format
    json data;
    data["status"] = response._status;
    data["statistics"] = highestScoresJson;

    // Serialize json packet
    string jsonStr = data.dump();

    // Places the msg code as the first byte
    buffer.push_back(GET_HIGH_SCORE_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {
        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }
    return buffer;
}

/*
This function serializes the getPersonalStats response to send back to a client, after receiving a valid getPersonalStats request.
Inputs: A GetPersonalStatsResponse struct variable that holds the status and the statistics to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const getPersonalStatsResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Create a json object for statistics
    json statisticsJson;
    statisticsJson["num_of_player_games"] = response._statistics[0];
    statisticsJson["num_of_correct_answers"] = response._statistics[1];
    statisticsJson["num_of_wrong_answers"] = response._statistics[2];
    statisticsJson["avg_answer_time"] = response._statistics[3];

    // Create an array of json objects containing the statistics object
    json personalStatsJson = json::array();
    personalStatsJson.push_back(statisticsJson);

    // Convert GetPersonalStatsResponse to JSON format
    json data;
    data["status"] = response._status;
    data["statistics"] = personalStatsJson;

    // Serialize JSON packet
    string jsonStr = data.dump();

    // Place the message code as the first byte
    buffer.push_back(GET_STATS_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Convert the data size to binary and push it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {
        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Push the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }
    return buffer;
}

/*
This function serializes the closeRoomResponse response to send back to a client, after receiving a valid closeRoomResponse request.
Inputs: A CloseRoomResponse struct variable that holds the status to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Coverts CloseRoomResponse to JSON format
    json data;
    data["status"] = response._status;

    // Serialize Json packet
    string jsonStr = data.dump();

    // Place the msg code as the first byte
    buffer.push_back(CLOSE_ROOM_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Covert the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {
        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message contet to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }

    return buffer;
}

/*
This function serializes the startGameResponse response to send back to a client, after receiving a valid startGameResponse request.
Inputs: A StartGameResponse struct variable that holds the status to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const StartGameResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Converts StartGameResponse to JSON format
    json data;
    data["status"] = response._status;

    // Serializes JSON packet
    string jsonStr = data.dump();

    // Places the msg code as the first byte
    buffer.push_back(START_GAME_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {

        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }

    return buffer;
}

/*
This function serializes the getRoomStateResponse response to send back to a client, after receiving a valid getRoomStateResponse request.
Inputs: A GetRoomStateResponse struct variable that holds the status, id the game has begun, the players, the count of the question and the answer's time out to return.
Output: A vector of bytes (unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Create an array of JSON object for rooms
    json playersJson = json::array();
    for (const auto& player : response._players)
    {
        playersJson.push_back(player);
    }

    // Convert GetRoomStateResponse to JSON format
    json data;
    data["status"] = response._status;
    data["hasGameBegun"] = response._hasGameBegun;
    data["isActive"] = response._isActive;
    data["players"] = playersJson;
    data["questionCount"] = response._questionCount;
    data["answerTimeout"] = response._answerTimeout;

    // Serailize json packet
    string jsonStr = data.dump();

    // Places the msg code as the first byte
    buffer.push_back(GET_ROOM_STATE_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Covert the data size to binary and pushes it to the vector aloing 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {

        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }

    return buffer;


}

/*
This function serializes the leaveRoomResponse response to send back to a client, after receiving a valid leaveRoomResponse request.
Inputs: A LeaveRoomResponse struct variable that holds the status to return.
Output : A vector of bytes(unsigned chars) contains the serialized response.
*/
vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse response)
{
    vector<unsigned char> buffer;
    int dataSize = 0;

    // Converts LeaveRoomResponse to JSON format
    json data;
    data["status"] = response._status;

    // Serializes JSON packet
    string jsonStr = data.dump();

    // Places the msg code as the first byte
    buffer.push_back(LEAVE_ROOM_RESP_CODE);

    // Size of the message
    dataSize = jsonStr.size();

    // Converts the data size to binary and pushes it to the vector along 4 bytes
    for (int i = MSG_LEN - 1; i >= 0; i--)
    {

        buffer.push_back((dataSize >> (i * 8)) & BYTE_MASK);
    }

    // Pushes the message content to the bytes vector
    for (char c : jsonStr)
    {
        buffer.push_back(c);
    }

    return buffer;
}
