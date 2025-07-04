#include "JsonRequestPacketDeserializer.h"

/*
This function deserializes the login request.
Inputs: A vector of bytes (unsigned chars) contains the login request data.
Output: A LoginRequest struct variable that has all the needed data for a login request ( name and password )
*/
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const vector<unsigned char>& buffer)
{
	LoginRequest request; // make a new reuqest based on the class we made
	json data;
	// Extracting the JSON data
	string jsonString(buffer.begin(), buffer.end()); 

	// Parses the JSON data
	data = json::parse(jsonString);

	// Gets the username and the password from the JSON data
	request.m_username = data["username"];
	request.m_password = data["password"];

	return request; // return the proper login request
}

/*
This function deserializes the signup request.
Inputs: A vector of bytes (unsigned chars) contains the signup request data.
Output: A SignupRequest struct that has all the needed data for a signup ( such as name, password and email ).
*/
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const vector<unsigned char>& buffer)
{

	SignupRequest request;
	unsigned int dataSize = 0;
	json data;

	// Extracting the JSON data
	string jsonString(buffer.begin(), buffer.end());

	// Parses the JSON data 
	data = json::parse(jsonString);

	// Extracts the username, the password and the email from JSON data
	request.m_username = data["username"];
	request.m_password = data["password"];
	request.m_email = data["email"];

	return request; // and return the proper signup request
}

/*
This function deserializes the getPlayers request received from a conversation socket with one of the clients.
Inputs: A vector of bytes (unsigned chars) contains the getPlayers request data.
Output: A GetPlayersInRoomRequest struct variable that holds the id of the room that was sent in the request.
*/
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const vector<unsigned char>& buffer)
{
	GetPlayersInRoomRequest request;
	unsigned int dataSize = 0;
	json data;

	// Extract JSON data
	string jsonStr(buffer.begin(), buffer.end());

	// Parses json data
	data = json::parse(jsonStr);

	// Extracts roomId from JSON data
	request._roomId = data["roomId"];
	
	return request;
}

/*
This function deserializes the joinRoom request received from a conversation socket with one of the clients.
Inputs: A vector of bytes (unsigned chars) contains the joinRoom request data.
Output: A JoinRoomRequest struct variable that holds the id of the room that was sent in the request.
*/
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const vector<unsigned char>& buffer)
{
	JoinRoomRequest request;
	unsigned int dataSize = 0;
	json data;

	// Extact JSON data
	string jsonStr(buffer.begin(), buffer.end());

	// Parses json data
	data = json::parse(jsonStr);

	// Extracts roomId from JSON data
	request._roomId = data["roomId"];

	return request;
}

/*
This function deserializes the createRoom request received from a conversation socket with one of the clients.
Inputs: A vector of bytes (unsigned chars) contains the createRoom request data.
Output: A CreateRoomRequest struct variable that holds the name of the room, max users, count of questions and time to answer every question that was sent in the request.
*/
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const vector<unsigned char>& buffer)
{
	CreateRoomRequest request;
	unsigned int dataSize = 0;
	json data;

	// Extracts JSON data
	string jsonStr(buffer.begin(), buffer.end());

	// Parses JSON data
	data = json::parse(jsonStr);

	// Extracts roomName, maxUsers, questionCount, and answerTimeout from JSON data
	request._roomName = data["roomName"];
	request.maxUsers = data["maxUsers"];
	request._questionCount = data["questionCount"];
	request._answerTimeout = data["answerTimeout"];

	return request;
}
