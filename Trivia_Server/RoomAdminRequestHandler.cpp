#include "RoomAdminRequestHandler.h"
#include "LoggedUser.hpp"
#include "Communicator.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user, Room& room) : 
	m_handlerFactory(handlerFactory), m_room(room), m_user(user)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	if (reqInfo._id == CLOSE_ROOM_REQ_CODE || reqInfo._id == START_GAME_REQ_CODE || reqInfo._id == GET_ROOM_STATE_REQ_CODE)
	{
		return true;
	}

	return false;
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	RequestResult result;

	if (!this->isRequestRelevant(reqInfo))
	{
		// Stay on the same state in the state machine
		result._newHandler = this;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Wrong requset code" });
	}
	else
	{
		switch (reqInfo._id)
		{

		case RequestsCode::CLOSE_ROOM_REQ_CODE:
			result = this->closeRoom(reqInfo);
			break;
		case RequestsCode::START_GAME_REQ_CODE:
			result = this->startGame(reqInfo);
			break;
		case RequestsCode::GET_ROOM_STATE_REQ_CODE:
			result = this->getRoomState(reqInfo);
			break;
		}
	}

	return result;
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& reqInfo)
{
	RequestResult result;

	// Stays on the RoomAdmin state in the state-machine (for now until V4.0.0)
	result._newHandler = this;
	result._Buffer = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ 1 });

	// Indicate that the game has started
	this->m_room.SetHasGameBegun(true);

	return result;
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& reqInfo)
{
	RequestResult result;

	// Gets back to the menu state in the state-machine
	result._newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	result._Buffer = JsonResponsePacketSerializer::serializeResponse(CloseRoomResponse{ 1 });

	// Remove all users
	vector<string> roomUsers = this->m_room.getAllUsers();
	for (const auto& user : roomUsers)
	{
		this->m_room.removeUser(LoggedUser(user));
	}

	this->m_handlerFactory.getRoomManager().deleteRoom(this->m_room.getRoomData()._id);

	return result;
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& reqInfo)
{
	RequestResult result;
	RoomData data = this->m_room.getRoomData();
	GetRoomStateResponse stateResponse;

	stateResponse._status = 1;
	stateResponse._hasGameBegun = data._hasGameBegun;
	stateResponse._isActive = data._isActive;
	stateResponse._questionCount = data._numOfQuestions;
	stateResponse._players = this->m_room.getAllUsers();
	stateResponse._answerTimeout = data._timePerQuestion;

	// If bug happened in V3.0.0 keep an eye on this code
	if (stateResponse._hasGameBegun == GAME_STARTED)
	{
		result = startGame(reqInfo);
	}
	else
	{
		result._newHandler = this;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(stateResponse);
	}

	return result;
}

LoggedUser RoomAdminRequestHandler::getUser() const
{
	return this->m_user;
}


