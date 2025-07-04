#pragma once
#include <string>

using std::string;


class LoggedUser
{
public:
	LoggedUser(const string& username) : m_username(username)
	{}

	string getUserName() const
	{
		return this->m_username;
	}
private:
	string m_username;
};