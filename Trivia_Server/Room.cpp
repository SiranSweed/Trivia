#include "Room.h"
#include <exception>

/*
This is the class Ctor - prepares a new room object
*/
Room::Room(const RoomData& roomData) : m_metadata(roomData)
{
}

/*
This fucntion adds a new user to the room (only if he doesn't exist already)
*/
void Room::addUser(const LoggedUser& useer)
{
	if (this->m_metadata._maxPlayers == this->m_users.size())
	{
		throw std::exception("The room is full");
	}

	bool existsAlready = false;
	for (auto it = this->m_users.begin(); it != this->m_users.end() && !existsAlready; it++)
	{
		if (it->getUserName() == useer.getUserName())
		{
			existsAlready = true;
		}
	}

	if (!existsAlready)
	{
		m_users.push_back(useer);
	}
	else
	{
		throw std::exception("The user already exists in the room");
	}
}

/*
This function deletes a user from the room only if he exists
*/
void Room::removeUser(const LoggedUser& user)
{
	for (auto it = this->m_users.begin(); it != this->m_users.end(); it++)
	{
		if (it->getUserName() == user.getUserName())
		{
			m_users.erase(it);
			return;
		}
	}
}

/*
This function return a vector that contains the string username of the users 
*/
vector<string> Room::getAllUsers() const
{
	vector<string> allUsers;
	for (auto it = m_users.begin(); it != m_users.end(); it++)
	{
		allUsers.push_back(it->getUserName());
	}
	return allUsers;
}

/*
This function sets the current status of the room
*/
//void Room::setStatus(const RoomStatus& status)
//{
//	this->m_metadata._status = status;
//}
//
//void Room::setHasGameBegun(const bool statusOfGame)
//{
//	 m_metadata._hasGameBegun = statusOfGame;
//}
//
//RoomStatus Room::getRoomStatus() const
//{
//	return m_metadata._status;
//}
//
//bool Room::getGameStatus() const
//{
//	return m_metadata._hasGameBegun;
//}

/*
This function returns the state of the room.
Inputs: None.
Output: The state of the room.
*/
unsigned int Room::getRoomState() const
{
	return this->m_metadata._isActive;
}

/*
This function returns the data of the room.
Inputs: None.
Output: The data of the room.
*/
RoomData Room::getRoomData() const
{
	return m_metadata;
}

void Room::SetHasGameBegun(bool hasGameStarted)
{
	this->m_metadata._hasGameBegun = hasGameStarted;
}

void Room::SetIsActive(bool isActive)
{
	this->m_metadata._isActive = isActive;
}
