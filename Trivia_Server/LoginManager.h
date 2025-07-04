#pragma once

#include "IDatabase.h"
#include "SqliteDatabase.h"
#include "LoggedUser.hpp"
#include <vector>

using std::vector;

class LoginManager
{
public:

	LoginManager(IDatabase* database);
	~LoginManager() = default;

	void signup(const string& username, const string& password, const string& email);
	void login(const string& username, const string& password);
	void logout(const string& username);

private:
	// Fileds
	IDatabase* m_database;
	vector<LoggedUser> m_loggedUsers;
};
