#pragma once

#include "RoomData.h"
#include "LoggedUser.hpp"
#include <vector>

using std::vector;

class Room
{
public:
	Room() = default;
	Room(const RoomData& roomData);
	~Room() = default;
	void addUser(const LoggedUser& useer);
	void removeUser(const LoggedUser& user);
	vector<string> getAllUsers() const;

	// We added setters and getters
	/*void setStatus(const RoomStatus& status);
	void setHasGameBegun(const bool statusOfGame);
	RoomStatus getRoomStatus() const;
	bool getGameStatus() const;
	RoomData getRoomData() const;*/

	// Our additions
	unsigned int getRoomState() const;
	RoomData getRoomData() const;
	void SetHasGameBegun(bool hasGameStarted);
	void SetIsActive(bool isActive);

private:
	RoomData m_metadata;
	vector<LoggedUser> m_users;
};

