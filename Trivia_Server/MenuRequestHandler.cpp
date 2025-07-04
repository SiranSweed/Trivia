#include "MenuRequestHandler.h"
#include <exception>

using std::exception;

/*
This is the class' CTOR - prepares a new MenuRequestHandler
object by initializing the user and handler factory.
Inputs: A LoggedUser struct to init with the m_user field, such as handlerFactory to init with the m_handlerFactory field.
*/
MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& handlerFactory, const LoggedUser& user) : m_handlerFactory(handlerFactory), m_user(user)
{
}

/*
This function checks if the request code is menu (and not something else).
Inputs: A struct holds the request information.
Output: True if the request is menu / False - else.
*/
bool MenuRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	if (reqInfo._id == RequestsCode::CREATE_ROOM_REQ_CODE ||
		reqInfo._id == RequestsCode::GET_ROOMS_REQ_CODE ||
		reqInfo._id == RequestsCode::GET_PLAYERS_IN_ROOM_REQ_CODE ||
		reqInfo._id == RequestsCode::JOIN_ROOM_REQ_CODE ||
		reqInfo._id == RequestsCode::GET_STATS_REQ_CODE ||
		reqInfo._id == RequestsCode::GET_HIGH_SCORES_REQ_CODE ||
		reqInfo._id == RequestsCode::LOGOUT_REQ_CODE)
	{
		return true;
	}
	
	return false;
}

/*
This function handles with auxiliary function the given request with an appropriate response.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult MenuRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	if (!this->isRequestRelevant(reqInfo))
	{
		result._newHandler = this;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Wrong request code" });
	}
	else // Valid request code
	{
		switch (reqInfo._id)
		{
		case RequestsCode::CREATE_ROOM_REQ_CODE:
			result = createRoom(reqInfo);
			break;

		case RequestsCode::GET_ROOMS_REQ_CODE:
			result = getRooms(reqInfo);
			break;

		case RequestsCode::GET_PLAYERS_IN_ROOM_REQ_CODE:
			result = getPlayersInRoomRequest(reqInfo);
			break;

		case RequestsCode::JOIN_ROOM_REQ_CODE:
			result = joinRoom(reqInfo);
			break;

		case RequestsCode::GET_STATS_REQ_CODE:
			result = getPersonalStats(reqInfo);
			break;

		case RequestsCode::LOGOUT_REQ_CODE:
			result = logout(reqInfo);
			break;

		case RequestsCode::GET_HIGH_SCORES_REQ_CODE:
			result = getHighScore(reqInfo);
			break;
		}
	}

	return result;
}

/*
This function operates a logout by sending back a serialized logout response - the
communicator class will check it and know to stop the communication and release resources.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult MenuRequestHandler::logout(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		this->m_handlerFactory.getLoginManager().logout(this->m_user.getUserName());

		// Leave the state machine
		result._newHandler = nullptr;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(LogoutResponse{ 1 });
	}
	catch (exception& e)
	{
		// If an error occured, stays on the same state (MenuRequestHandler)
		result._newHandler = this;
		// Send an error message back
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}

	return result;
}

/*
This function calls the function from the room manager that is in
the handler factory, to find all the rooms exist in the game at the moment.
Then serialize a response according to the success in the progress.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult MenuRequestHandler::getRooms(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		// Stay on the same state in the state-matchine
		result._newHandler = this;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(GetRoomsResponse{ 1, this->m_handlerFactory.getRoomManager().getRooms() });
	}
	catch (exception& e)
	{
		// If an error occured, stays on the same state (MenuRequestHandler)
		result._newHandler = this;
		// Send an error message back
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}

	return result;
}

/*
This function calls the functions from the room manager that is in
the handler factory, to get a room by its id, followed by finding the players in this room.
Then serialize a response according to the success in the progress.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult MenuRequestHandler::getPlayersInRoomRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		GetPlayersInRoomRequest getPlayersReq = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(reqInfo._buffer);
		Room& roomToGetPlayers = this->m_handlerFactory.getRoomManager().getRoom(getPlayersReq._roomId);

		// Stays on the menu state in the state-machine
		result._newHandler = this;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse{ roomToGetPlayers.getAllUsers() });
	}
	catch (exception& e)
	{
		// If an error occured, stays on the same state (MenuRequestHandler)
		result._newHandler = this;
		// Send an error message back
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}

	return result;
}

/*
This function calls the function from the statistics manager that is in
the handler factory, to get this player's preferences.
Then serialize a response according to the success in the progress.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		// Stay on the same state on the state-machine
		result._newHandler = this;
		std::vector<string> statistics = this->m_handlerFactory.getStatisticsManager().getUserStatistics(this->m_user.getUserName());
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(getPersonalStatsResponse{ 1, statistics });
	}
	catch (exception& e)
	{
		// If an error occured, stays on the same state (MenuRequestHandler)
		result._newHandler = this;
		// Send an error message back
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}
	
	return result;
}

/*
This function calls the function from the statistics manager that is in
the handler factory, to get the top players and their scores.
Then serialize a response according to the success in the progress.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult MenuRequestHandler::getHighScore(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		// Stay on the same state on the state-machine
		result._newHandler = this;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(getHightScoreResponse{ 1, this->m_handlerFactory.getStatisticsManager().getHighScores() });
	}
	catch (exception& e)
	{
		// If an error occured, stays on the same state (MenuRequestHandler)
		result._newHandler = this;
		// Send an error message back
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}

	return result;
}

/*
This function operates joining to a game room by getting the requested room from the room
manager (that is in the handler factory), and then adding this user to the found room.
Then serialize a response according to the success in the progress.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult MenuRequestHandler::joinRoom(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		JoinRoomRequest joinRoomReq = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(reqInfo._buffer);
		Room& roomToJoin = this->m_handlerFactory.getRoomManager().getRoom(joinRoomReq._roomId);
		roomToJoin.addUser(this->m_user);

		result._newHandler = this->m_handlerFactory.createRoomMemberRequestHandler(this->m_user, roomToJoin); // move to the RoomMember state
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(JoinRoomResponse{ 1 });
	}
	catch (exception& e)
	{
		// If an error occured, stays on the same state (MenuRequestHandler)
		result._newHandler = this;
		// Send an error message back
		std::cerr << e.what();
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}

	return result;
}

/*
This function calls the function from the room manager that is in
the handler factory, to create a new room with the request info.
Then serialize a response according to the success in the progress.
Inputs: A struct holds the request information.
Output: A struct holds the response (after serialization) that matches the given request.
*/
RequestResult MenuRequestHandler::createRoom(const RequestInfo& reqInfo)
{
	RequestResult result;

	try
	{
		CreateRoomRequest createRoomReq = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(reqInfo._buffer);
		// Get the room reference for the admin in future
		Room& room = this->m_handlerFactory.getRoomManager().createRoom(this->m_user, RoomData{MEANINGLESS_ID, createRoomReq._roomName, createRoomReq.maxUsers, createRoomReq._questionCount, createRoomReq._answerTimeout, true, false});

		result._newHandler = this->m_handlerFactory.createRoomAdminRequestHandler(this->m_user, room); // Move to the Room Admin state
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(CreateRoomResponse{ 1 });
	}
	catch (exception& e)
	{
		// If an error occurred, stays on the same state (MenuRequestHandler)
		result._newHandler = this;
		// Sends back an error response with the exception description as a message
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ string(e.what()) });
	}

	return result;
}
