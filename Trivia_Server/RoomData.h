#pragma once

#include <string>

using std::string;

enum RoomStatus {Closed, Opened};

typedef struct RoomData
{
	unsigned int _id;
	string _name;
	unsigned int _maxPlayers;
	unsigned int _numOfQuestions;
	unsigned int _timePerQuestion;
	unsigned int _isActive;
	unsigned int _hasGameBegun; // this field helps us find if the game is begin played or not

}RoomData;

