#include "RoomMemberRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(RequestHandlerFactory& handlerFactory, LoggedUser user, Room& room) :
	m_handlerFactory(handlerFactory), m_room(room), m_user(user)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	if (reqInfo._id == LEAVE_ROOM_REQ_CODE || reqInfo._id == GET_ROOM_STATE_REQ_CODE)
	{
		return true;
	}

	return false;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& reqInfo)
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

		case RequestsCode::LEAVE_ROOM_REQ_CODE:
			result = this->leaveRoom(reqInfo);
			break;

		case RequestsCode::GET_ROOM_STATE_REQ_CODE:
			result = this->getRoomState(reqInfo);
			break;
		}
	}

	return result;
}

LoggedUser RoomMemberRequestHandler::getUser() const
{
	return this->m_user;
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& reqInfo)
{
	RequestResult result;

	// Gets back to the menu state in the state-machine
	result._newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
	result._Buffer = JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse{ 1 });

	this->m_room.removeUser(this->m_user);

	return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& reqInfo)
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

	if (stateResponse._isActive == ROOM_WAS_CLOSED)
	{
		// Gets back to the menu state in the state-machine
		result._newHandler = this->m_handlerFactory.createMenuRequestHandler(this->m_user);
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(stateResponse);
	}
	else if (stateResponse._hasGameBegun == GAME_STARTED)
	{
		result = startGame(reqInfo);
	}
	else
	{
		// Stays on the RoomMember state in the state-machine
		result._newHandler = this;
		result._Buffer = JsonResponsePacketSerializer::serializeResponse(stateResponse);
	}

	return result;
}

RequestResult RoomMemberRequestHandler::startGame(const RequestInfo& reqInfo)
{
	RequestResult result;

	// Stays on the RoomMember state in the state-machine (for this version)
	result._newHandler = this;
	result._Buffer = JsonResponsePacketSerializer::serializeResponse(StartGameResponse{ 1 });


	return result;
}




