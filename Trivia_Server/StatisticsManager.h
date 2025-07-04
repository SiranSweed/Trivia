#pragma once

#include "IDataBase.h"
#include "SqliteDatabase.h"
#include <string>
#include <vector>

using std::string;

class StatisticsManager
{
public:
	// Ctor and Dtor
	StatisticsManager(IDatabase* database);
	~StatisticsManager() = default;

	// Methods
	std::vector<string> getHighScores();
	std::vector<string> getUserStatistics(const string& username);

private:
	IDatabase* m_database;
};
