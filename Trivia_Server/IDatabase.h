#pragma once

#include <string>
#include "Question.h"
#include <list>
#include <vector>

using std::string;
using std::vector;

class IDatabase
{
public:

	// Ctor and Dtor default
	IDatabase() = default;
	virtual ~IDatabase() = default;

	virtual bool open() = 0;
	virtual bool close() = 0;
	
	virtual bool doesUserExist(const string& user) = 0;
	virtual bool doesPasswordMatch(const string& user, const string& password) = 0;

	virtual bool addNewUser(const string& user, const string& password, const string& email) = 0;
	virtual bool addUserStatistics(const string& username) = 0;
	virtual bool addQuestion(string& question, string& right_answer, std::vector<string>& wrong_answers) = 0;


	virtual const Question getQuestion(const int questionId) = 0;
	virtual const Question getQuestion(const string& question) = 0;
	virtual const std::list<Question> getQuestions(const int amount) = 0;
	virtual const int getAmountOfQuestion() = 0;
	virtual const vector<string> getListOfUsernames() = 0;

	// Statistics functions
	virtual const float getPlayerAverageAnswerTime(const string& username) = 0;
	virtual const int getNumOfCorrectAnswers(const string& username) = 0;
	virtual const int getNumOfWrongAnswers(const string& username) = 0;
	virtual const int getNumOfTotalAnswers(const string& username) = 0;
	virtual const int getNumOfPlayerGames(const string& username) = 0;
	virtual const int getPlayerScore(const string& username) = 0;
	virtual const vector<string> getHighScores() = 0;

	virtual const void setAverageAnswerTime(const string username, const float avgTime) = 0;
	virtual const void setNumOfCorrectAnswers(const string username, const int numCorrectAnswers) = 0;
	virtual const void setNumOfWrongAnswers(const string username, const int numWrongAnswers) = 0;
	virtual const void setNummOfPlayerGames(const string username, const int numGames) = 0;

};
