#pragma once

#include "Room.h"
#include <map>

#define GAME_STARTED true
#define ON_WAITING_ROOM false
#define ROOM_IS_ACTIVE true
#define ROOM_WAS_CLOSED false

class RoomManager
{
public:
	// Ctor and Dtor
	RoomManager();
	~RoomManager();

	// Class methods
	Room& createRoom(const LoggedUser& user, const RoomData& roomData);
	void deleteRoom(const int ID);

	// This function returns if the room is closed or Opened
	unsigned int getRoomState(int ID);

	vector<RoomData> getRooms() const;
	Room& getRoom(const int ID);
	int getNextRoomID() const;

private:
	// The rooms
	std::map<int, Room> m_rooms; // id - room
};
