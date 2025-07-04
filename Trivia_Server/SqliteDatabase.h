#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <iostream>
#include <io.h>
#include <functional>
#include <vector>
#include <list>
#include <algorithm>

#define DB_FILE_NAME "Trivia.db"
#define MISSING_FILE_CODE -1


static int callBackQuestions(void* _data, int argc, char** argv, char** azColName);
static int callBackInt(void* _data, int argc, char** argv, char** azColName);
static int caallBackFloat(void* _data, int argc, char** argv, char** azColName);
static int callBackNames(void* _data, int argc, char** argv, char** azColName);


class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();
	virtual ~SqliteDatabase() = default;

	virtual bool open() override;
	virtual bool close() override;

	virtual bool doesUserExist(const string& user) override;
	virtual bool doesPasswordMatch(const string& user, const string& password) override;

	virtual bool addNewUser(const string& user, const string& password, const string& email) override;
	virtual bool addUserStatistics(const string& username) override;
	virtual bool addQuestion(string& question, string& right_answer, std::vector<string>& wrong_answers) override;


	virtual const Question getQuestion(const int questionId) override;
	virtual const Question getQuestion(const string& question) override;
	virtual const std::list<Question> getQuestions(const int amount) override;
	virtual const int getAmountOfQuestion() override;
	virtual const std::vector<string> getListOfUsernames() override;

	// Statistics functions
	virtual const float getPlayerAverageAnswerTime(const string& username) override;
	virtual const int getNumOfCorrectAnswers(const string& username) override;
	virtual const int getNumOfWrongAnswers(const string& username) override;
	virtual const int getNumOfTotalAnswers(const string& username) override;
	virtual const int getNumOfPlayerGames(const string& username) override;
	virtual const int getPlayerScore(const string& username) override;
	virtual const std::vector<string> getHighScores() override;

	// Setter functions for the table in the database
	virtual const void setAverageAnswerTime(const string username, const float avgTime) override;
	virtual const void setNumOfCorrectAnswers(const string username, const int numCorrectAnswers) override;
	virtual const void setNumOfWrongAnswers(const string username, const int numWrongAnswers) override;
	virtual const void setNummOfPlayerGames(const string username, const int numGames) override;

private:

	/* Create database tables*/
	void createDataBase();

	/* These functions executes the querys */
	bool executeQuery(std::string query, std::function<void(std::vector<std::vector<std::string>>)> resultCallBack);
	bool executeQuery(std::string query, int (*callback)(void*, int, char**, char**), void* data);

	sqlite3* _TriviaDB;
	
	std::vector<std::string> _sql_result;
};
