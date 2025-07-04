#include "LoginManager.h"
#include <iostream>
#include <exception>
#include <algorithm>

using std::cerr;
using std::endl;
using std::exception;
using std::find_if;

/*
This is the class' CTOR - prepares a new LoginManager object
by allocating the m_database field with a given pointer to DB.
Inputs: A database to init with the m_database field.
*/
LoginManager::LoginManager(IDatabase* database) : m_database(database)
{
	m_loggedUsers = {};
}

/*
This function signs up a new user to the DB.
Inputs: A user info - username, password and email.
Output: None.
*/
void LoginManager::signup(const string& username, const string& password, const string& email)
{
	if (this->m_database->doesUserExist(username))
	{
		throw exception("A user with the same username already exists!!");
	}

	// If the user doesn't exist then add him
	if (!this->m_database->addNewUser(username, password, email))
	{
		throw exception("An error occurred while trying to add the user to the DB!!");
	}

	// Add the user to the vector of logged users if everything went fine
	this->m_loggedUsers.push_back(LoggedUser(username));
}

/*
This func logs on the user
Inputs: the users name & password
Output: None.
*/
void LoginManager::login(const string& username, const string& password)
{
	// the user should sign up before logging in
	if (!this->m_database->doesUserExist(username))
	{
		throw exception("The user does not exist");
	}

	// password is incorrect
	if (!this->m_database->doesPasswordMatch(username, password))
	{
		throw exception("Wrong password");
	}

	// Does user already exist ?
	auto it = find_if
	(
		this->m_loggedUsers.begin(),
		this->m_loggedUsers.end(),
		[&](const LoggedUser& user)
		{
			return user.getUserName() == username;
		}
	);

	if (it != this->m_loggedUsers.end())
	{
		throw exception("The user is already logged in");
	}


	// adding the user to the logged users vector
	this->m_loggedUsers.push_back(LoggedUser(username));
}

/*
This func logs out the user.
Inputs: the username of the user.
Output: None
*/
void LoginManager::logout(const string& username)
{
	// check if the user is in the database
	if (!this->m_database->doesUserExist(username))
	{
		throw exception("The user does not exist");
	}

	// check if the user in the vector of the logged users
	auto it = find_if
	(
		this->m_loggedUsers.begin(),
		this->m_loggedUsers.end(),
		[&](const LoggedUser& user)
		{
			return user.getUserName() == username;
		}
	);

	if (it != this->m_loggedUsers.end())
	{
		// remove the user if he is in the vector
		this->m_loggedUsers.erase(it);
	}
	else // if the user wasnt logged in
	{
		throw exception("The user was not logged in");
	}
}
