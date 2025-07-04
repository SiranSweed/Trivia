#include "RoomManager.h"
#include <exception>


RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

/*
This function creates a new room and adds the person who created the room while checking all the edge cases
*/
Room& RoomManager::createRoom(const LoggedUser& user, const RoomData& roomData)
{
	// Check the cases where the roomData is missing something or has false meanings
	if (roomData._maxPlayers == 0)
	{
		throw std::exception("Invalid number of max players");
	}
	if (roomData._numOfQuestions == 0)
	{
		throw std::exception("Inalid number of question zero!!");
	}
	if (roomData._timePerQuestion == 0)
	{
		throw std::exception("Invalid time per question zero!!");
	}
	if (roomData._name == "")
	{
		throw std::exception("Room name is empty");
	}

	// Create the room
	Room room = Room(roomData);

	// Add the user to be the first one (the admin)
	room.addUser(user);
	int roomID = getNextRoomID();
	// Add the room to the map
	this->m_rooms[roomID] = room;

	return m_rooms.at(roomID);
}

/*
This function deletes a room from the map by it's ID (by deleting we mean setting it as unactive)
*/
void RoomManager::deleteRoom(const int ID)
{
	for (auto it = m_rooms.begin(); it != m_rooms.end(); it++)
	{
		if (it->first == ID)
		{
			it->second.SetIsActive(false);
			return;
		}
	}
}

/*
This function returns if the room is closed or Opened
*/
unsigned int RoomManager::getRoomState(int ID)
{
	return this->m_rooms[ID].getRoomState();
}

/*
This fucntion returns a vector of all the room(the data of the rooms)
*/
vector<RoomData> RoomManager::getRooms() const
{
	vector<RoomData> activeRooms;
	for (auto it = this->m_rooms.begin(); it != this->m_rooms.end(); it++)
	{
		// If room is still active
		if (it->second.getRoomState() == RoomStatus::Opened)
		{
			activeRooms.push_back(it->second.getRoomData());
		}
	}
	return activeRooms;
}

Room& RoomManager::getRoom(const int ID)
{
	for (auto it = m_rooms.begin(); it != m_rooms.end(); it++)
	{
		if (it->first == ID)
		{
			return it->second;
		}
	}
	throw std::exception(("The Room with the ID: " + std::to_string(ID) + " Doesnt exist").c_str());
}

int RoomManager::getNextRoomID() const
{
	int result = 0;
	for (const auto& pair : this->m_rooms)
	{
		if (pair.first == result)
		{
			result++;
		}
		else
		{
			return result;
		}
	}
	return result;
}
