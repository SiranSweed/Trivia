#include "SqliteDatabase.h"
#include <exception>
using std::cout;
using std::endl;
using std::cerr;
using std::exception;

SqliteDatabase::SqliteDatabase()
{
	if (!this->open())
	{
		throw exception("Failed to open the DB");
	}
}

/*
This function opens the Database file.
Inputs: None.
Output:bool , True if succeded , False otherwise
*/
bool SqliteDatabase::open()
{
	int fileExist = _access(DB_FILE_NAME, 0);
	int execution = sqlite3_open(DB_FILE_NAME, &(this->_TriviaDB));

	if (execution != SQLITE_OK)
	{
		this->_TriviaDB = nullptr;
		cout << "Failed to open the database" << endl;
		return false;
	}

	// If the file (database file) doesn't exists then create it
	if (fileExist == MISSING_FILE_CODE)
	{
		this->createDataBase();
	}

	return true;

}

/*
This function creates the database tables
Input: none
Output: none
*/
void SqliteDatabase::createDataBase()
{
	std::cout << "Creating users table" << std::endl;
	std::string query = "";

	query = "CREATE TABLE Users (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT UNIQUE, email TEXT UNIQUE, password TEXT);";
	if (!executeQuery(query, NULL))
	{
		throw std::exception("[SQL ERROR] - Couldn't Create Users table");
	}

	query = "CREATE TABLE Questions (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, QUESTION TEXT NOT NULL, C_ANSWER1 TEXT NOT NULL, W_ANSWER1 TEXT NOT NULL, W_ANSWER2 TEXT NOT NULL, W_ANSWER3 TEXT NOT NULL);";
	if (!executeQuery(query, NULL))
	{
		throw std::exception("[SQL ERROR] - Couldn't Create Questions table");
	}

	query = "CREATE TABLE Statistics (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, USERNAME TEXT UNIQUE NOT NULL, AVG_TIME FLOAT NOT NULL, C_ANSWERS INTEGER NOT NULL, W_ANSWERS INTEGERS NOT NULL, GAMES INTEGER NOT NULL, FOREIGN KEY (Username) REFERENCES USERS(username));";
	if (!executeQuery(query, NULL))
	{
		throw std::exception("[SQL ERROR] - Couldn't create statistics table");
	}

	std::cout << "Done creating the database's tables" << std::endl;
}

/*
This function closes the database file
Inputs: none
Output: True if succeeded , False otherwise
*/
bool SqliteDatabase::close()
{
	int execution = 0;

	if (this->_TriviaDB == nullptr) // theres no DB
	{
		return false;
	}

	execution = sqlite3_close(this->_TriviaDB);
	this->_TriviaDB = nullptr; // Set it to null

	if (execution != SQLITE_OK)
	{
		// closing the database failed
		return false;
	}

	// closing the data base succeeded
	return true;
}

/*
This function checks if the user exists in the database tables
Inputs: the username
Output: if the user exists or not
*/
bool SqliteDatabase::doesUserExist(const string& user)
{
	std::string query = "SELECT * FROM Users WHERE username = '" + user + "';";
	bool result = false;

	// Execute the query
	executeQuery(query, [&result](std::vector<std::vector<std::string>> resultData)
		{
			// If there is any data the result will be false
			result = !resultData.empty();
		});

	return result;
}

/*
This function checks if the password matches any password in the database
Inputs: the username and the user's password
Output: If it matches any password or not
*/
bool SqliteDatabase::doesPasswordMatch(const string& user, const string& password)
{
	std::string query = "SELECT * FROM Users WHERE username = '" + user + "' AND password = '" + password + "';";
	bool result = false;

	// Execute the query
	executeQuery(query, [&result](std::vector<std::vector<std::string>> resultData)
		{
			// The result will be true if there is any data
			result = !resultData.empty();
		});

	return result;
}

/*
This function adds a new user to the database if he doesnt exists
Inputs: the username, password and email
Output: if the adding is a success
*/
bool SqliteDatabase::addNewUser(const string& user, const string& password, const string& email)
{
	std::string query = "INSERT INTO Users (username, email, password) VALUES ('" + user + "', '" + email + "', '" + password + "');";

	if (!executeQuery(query, NULL))
	{
		std::cerr << "[SQL ERROR] - Couldn't insert into Users!";
		return false;
	}
	return true;
}

/*
This function add the user statistics
Inputs: the username
Output: if the operation is succssfull
*/
bool SqliteDatabase::addUserStatistics(const string& username)
{
	std::string query = "INSERT INTO Statistics (USERNAME, AVG_TIME, C_ANSWERS, GAMES) VALUES ('" + username + "', 0, 0, 0, 0);";
	
	if (!executeQuery(query, NULL))
	{
		std::cerr << "[SQL ERROR] - Couldn't insert into Statistics!";
		return false;
	}
	return true;
}

/*
This function adds a new quesiton to the data
Inputs: the question , the right answer and the wrong answers
Output: if the operation succseded or not
*/
bool SqliteDatabase::addQuestion(string& question, string& right_answer, std::vector<string>& wrong_answers)
{
	std::string query = "INSERT INTO Questions (QUESTION, C_ANSWER1, W_ANSWER1, W_ANSWER2, W_ANSWER3) VALUES ('" + question + "', '" + right_answer + "', '" + wrong_answers.at(0) + "', '" + wrong_answers.at(1) + "', '" + wrong_answers.at(2) + "');";
	if (!executeQuery(query, NULL))
	{
		std::cerr << "[SQL ERROR] - Couldn't insert question!";
		return false;
	}

	return true;
}

/*
This fucntion returns the needed question by the id
Inputs: the question id
Output: the question required
*/
const Question SqliteDatabase::getQuestion(const int questionId)
{
	std::string question = "";

	std::string query = "SELECT * FROM Questions WHERE ID = '" + std::to_string(questionId) + "';";

	std::list<Question> questions;
	questions.clear();

	executeQuery(query, callBackQuestions, &question);

	return questions.front();
}

/*
This fucntion returns the needed question by the question contents
Inputs: the question itself
Output: the question required
*/
const Question SqliteDatabase::getQuestion(const string& question)
{
	std::string query = "SELECT * FROM Questions WHERE QUESTION = '" + question + "';";
	std::list<Question> questions;
	questions.clear();

	executeQuery(query, callBackQuestions, &questions);

	return questions.front();
}

/*
This fucntion returns the amount of question the user sent
Inputs: the amount of questions
Output: the questions needed
*/
const std::list<Question> SqliteDatabase::getQuestions(const int amount)
{
	try
	{
		std::list<Question> questions;
		questions.clear();
		
		std::string query = "SELECT * FROM Questions LIMIT " + std::to_string(amount) + ";";
		executeQuery(query, callBackQuestions, &questions);

		return questions;
	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		return std::list<Question>();
	}
}

/*
This function returns how much questions are there
Inputs: none
Output: the number of question that exists
*/
const int SqliteDatabase::getAmountOfQuestion()
{
	try
	{
		int size = 0;
		string query = "SELECT COUNT(*) FROM Questions;";
		executeQuery(query, callBackInt, &size);

		return size;
	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		return 0;
	}
}

/*
This function returns the list of the usernames
Inputs: none
Output: a vector containing the username
*/
const std::vector<string> SqliteDatabase::getListOfUsernames()
{
	std::vector<string> retVector;
	retVector.clear();

	string query = "SELECT USERNAME FROM Statistics;";
	executeQuery(query, callBackNames, &retVector);

	return retVector;
}

/*
This fuctnion retunrns the average answer time for the person
Inputs: the username of the person
Output: the averag tiem
*/
const float SqliteDatabase::getPlayerAverageAnswerTime(const string& username)
{
	string query = "SELECT AVG_TIME FROM Statistics WHERE USERNAME = '" + username + "';";
	float result = 0;

	executeQuery(query, caallBackFloat, &result);
	
	return result;
}

/*
This fucntion returns how much correct answers the person have
Inputs: the username of the person
Output: the number of correct answers
*/
const int SqliteDatabase::getNumOfCorrectAnswers(const string& username)
{
	const int ERROR_SIGN = -1;

	string query = "SELECT C_ANSWERS FROM Statistics WHERE USERNAME = '" + username + "';";
	int result = 0;

	executeQuery(query, callBackInt, &result);

	return result;
}

/*
This fucntion returns how much wrong answers the person have
Inputs: the username of the person
Output: the number of wrong answers
*/
const int SqliteDatabase::getNumOfWrongAnswers(const string& username)
{
	string query = "SELECT W_ANSWERS FROM Statistics WHERE USERNAME = '" + username + "';";
	int result = 0;

	executeQuery(query, callBackInt, &result);

	return result;
}

/*
This function returns the total answers of the person
Inputs: the username
Output: the Total answers
*/
const int SqliteDatabase::getNumOfTotalAnswers(const string& username)
{
	return getNumOfCorrectAnswers(username) + getNumOfWrongAnswers(username);
}

/*
This function returns the number of games the user played
Inputs: the username
Output: the number of games played by the user
*/
const int SqliteDatabase::getNumOfPlayerGames(const string& username)
{
	string query = "SELECT GAMES FROM Statistics WHERE USERNAME = '" + username + "';";
	int result = 0;

	executeQuery(query, callBackInt, &result);

	return result;
}

/*
This function caluclates the player score, according to the formula:
(corrects / total) * games / avgTime * 1000. (percentage of correct answers of total quests in all games / avgTime * 1000 (to not be decimal to be readable number)
Inputs: the username
Output: the score of the player, or 0 if the user hasn't played any game
*/
const int SqliteDatabase::getPlayerScore(const string& username)
{
	float playerAvgAnsTime = this->getPlayerAverageAnswerTime(username);
	int numOfCorrectAnswers = this->getNumOfCorrectAnswers(username);
	int numOfTotalAnswers = this->getNumOfTotalAnswers(username);
	int numOfPlayerGames = this->getNumOfPlayerGames(username);

	// Prevents divsion by zero (+these values would occur only if the player hasn't played any game)
	if (playerAvgAnsTime == 0 || numOfPlayerGames == 0)
	{
		return 0;
	}

	return ((numOfCorrectAnswers / numOfTotalAnswers) * numOfPlayerGames) / playerAvgAnsTime * 1000;
}

/*
This function returns the high scores in the game
Inputs: none
Output: a vector that contains the string - each string has the format of "<Username>  -  <Score>".
*/
const std::vector<string> SqliteDatabase::getHighScores()
{
	const int NUM_TOP_PLAYERS = 5;
	const string SEPARATOR = "  -  ";

	std::vector<string> highScores;
	std::vector<string> usernames = this->getListOfUsernames();
	std::vector<std::pair<string, int>> userScores; // user name and his score

	for (const auto& username : usernames)
	{
		// Add the to the vector scores the player username and his score
		int score = this->getPlayerScore(username);
		if (score != 0)
		{
			userScores.emplace_back(username, score);
		}
		else
		{
			highScores.clear();
			return std::vector<string>();
		}
	}

	std::sort(userScores.begin(), userScores.end(), [](const std::pair<string, int>& a, const std::pair<string, int>& b)
		{
			return b.second < a.second; // Sort in descending order of scores
		});

	// Take the top 5 players and put their scores in it
	for (int i = 0; i < std::min((int)userScores.size(), NUM_TOP_PLAYERS); i++)
	{
		highScores.push_back(userScores[i].first + SEPARATOR + std::to_string(userScores[i].second));
	}

	return highScores;
}

/*
This function sets the average time on the Statistics table in the db
Inputs: the username and the avgtime
Output: None.
*/
const void SqliteDatabase::setAverageAnswerTime(const string username, const float avgTime)
{
	string query = "UPDATE Statistics SET AVG_TIME = " + std::to_string(avgTime) + " WHERE USERNAME = '" + username + "';";
	executeQuery(query, NULL);
}

/*
This function sets the correct answers on the Statistics table in the db
Inputs: the username and the number correctAnswers
Output: None.
*/
const void SqliteDatabase::setNumOfCorrectAnswers(const string username, const int numCorrectAnswers)
{
	string query = "UPDATE Statistics SET C_ANSWERS = " + std::to_string(numCorrectAnswers) + " WHERE USERNAME = '" + username + "';";
	executeQuery(query, NULL);
}

/*
This function sets the wrong answers on the Statistics table in the db
Inputs: the username and the number of wrong answers
Output: None.
*/
const void SqliteDatabase::setNumOfWrongAnswers(const string username, const int numWrongAnswers)
{
	string query = "UPDATE Statistics SET W_ANSWERS = " + std::to_string(numWrongAnswers) + " WHERE USERNAME = '" + username + "';";
	executeQuery(query, NULL);
}

/*
This function sets the number of games the user played on the Statistics table in the db
Inputs: the username and the avgtime
Output: None.
*/
const void SqliteDatabase::setNummOfPlayerGames(const string username, const int numGames)
{
	string query = "UPDATE Statistics SET GAMES = " + std::to_string(numGames) + " WHERE USERNAME = '" + username + "';";
	executeQuery(query, NULL);
}

/*
This function executes the sql query
Inputs: the query string to excute, a function to process the results rows This callback takes a 2D vector of strings, where each inner vector represents a row.
Behavior:
For SELECT-like queries: collects the result rows and invokes the resultCallBack with the data.
For INSERT/UPDATE/DELETE queries: executes the query without using the resultCallBack.
If the query fails, logs the error and returns false.
Output: if the query excuted or not
*/
bool SqliteDatabase::executeQuery(std::string query, std::function<void(std::vector<std::vector<std::string>>)> resultCallBack)
{
	std::vector<std::vector<std::string>> resultData;

	char* errorMessage = nullptr;
	// Use callback function to process each row and save it in the results
	int result = sqlite3_exec(this->_TriviaDB, query.c_str(), [](void* data, int argc, char** argv, char**)
		{
		auto resultData = reinterpret_cast<std::vector<std::vector<std::string>>*>(data);
		std::vector<std::string> row;
		// Iterate over the rows and save it (if there is data there and if there is null put null)
		for (int i = 0; i < argc; ++i)
		{
			row.push_back(argv[i] ? argv[i] : "NULL");
		}
		resultData->push_back(row);
		return 0; // return 0 if the excution successed then int result = 0
		}, &resultData, // Pass the result data to the callback that we used
		&errorMessage);

	if (result != SQLITE_OK)
	{
		std::cerr << "Error executing SQL statement: " << errorMessage << std::endl;
		sqlite3_free(errorMessage);
		return false;
	}

	// If the function was provided (wasnt NULL) then use it with the result data
	if (resultCallBack)
	{
		resultCallBack(resultData);
	}

	return true;

}

/*
This function executes the query that will be sent
*/
bool SqliteDatabase::executeQuery(std::string query, int(*callback)(void*, int, char**, char**), void* data)
{
	char* error_message = nullptr;

	bool sql_ok = sqlite3_exec(_TriviaDB, query.c_str(), callback, data, &error_message);
	if (error_message != nullptr)
	{
		cerr << "[SQL ERROR] - " << error_message << endl;
	}

	return sql_ok;
}

int callBackQuestions(void* _data, int argc, char** argv, char** azColName)
{
	auto& questions = *static_cast<std::list<Question>*>(_data);
	std::vector<std::string> wrongAnswers;
	wrongAnswers.clear();

	string correctAnswer, question;

	for (int i = 0; i < argc; i++)
	{
		if (std::string(azColName[i]) == "QUESTION")
		{
			question = argv[i];
		}
		else if (std::string(azColName[i]) == "C_ANSWER1")
		{
			correctAnswer = argv[i];
		}
		else if (std::string(azColName[i]).find("W_ANSWER") != string::npos)
		{
			wrongAnswers.push_back(argv[i]);
		}
	}

	Question q(question, wrongAnswers, correctAnswer);
	questions.push_back(q);

	return 0;
}

int callBackInt(void* _data, int argc, char** argv, char** azColName)
{
	auto& retInt = *static_cast<int*>(_data);

	// If there was an argument
	if (argc == 1 && argv[0] != nullptr)
	{
		retInt = std::atoi(argv[0]);
		return 0;
	}
	else
	{
		retInt = -1;
		return 1;
	}
}

int caallBackFloat(void* _data, int argc, char** argv, char** azColName)
{
	auto& retFloat = *static_cast<float*>(_data);

	// If there was an argument
	if (argc == 1 && argv[0] != nullptr)
	{
		retFloat = std::stof(argv[0]);
		return 0;
	}
	else
	{
		return 1;
	}
}

int callBackNames(void* _data, int argc, char** argv, char** azColName)
{
	auto& retVector = *static_cast<std::vector<string>*>(_data);

	for (int i = 0; i < argc; i++)
	{
		if (string(azColName[i]) == "Username")
		{
			retVector.push_back(argv[i]);
		}
	}

	return 0;
}
