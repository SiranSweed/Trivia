#include "StatisticsManager.h"

/*
This is the class C'tor - prepares a new statistic manager object
*/
StatisticsManager::StatisticsManager(IDatabase* database) : m_database(database)
{
}

/*
This function uses the DB class access the data base and get the top 5 players (or less if there are less than 5 players)
who gained the highest score in the system.
Inputs: none
Output: a Vector of string - each string contains a user's score in this format "<Username>  -  <Score>"
*/
std::vector<string> StatisticsManager::getHighScores() 
{
	return this->m_database->getHighScores();
}

/*
This function uses the DB class to access the database and collect personal stats by the username.
Inputs: A username to get performances according to.
Output: a vector of 4 strings - The number of games the player has played, the number of correct answers, the number of wrong answer, and the average time for answer
*/
std::vector<string> StatisticsManager::getUserStatistics(const string& username)
{
	std::vector<string> personalStatistics;
	int numOfPlayerGames = this->m_database->getNumOfPlayerGames(username);
	int numOfCorrectAnswers = this->m_database->getNumOfCorrectAnswers(username);
	int numOfWrongAnswers = this->m_database->getNumOfWrongAnswers(username);
	int numOfTotalAnswers = this->m_database->getNumOfTotalAnswers(username);
	float playerAvgAnsTime = this->m_database->getPlayerAverageAnswerTime(username);

	personalStatistics.push_back(std::to_string(numOfPlayerGames));
	personalStatistics.push_back(std::to_string(numOfCorrectAnswers));
	personalStatistics.push_back(std::to_string(numOfWrongAnswers));
	personalStatistics.push_back(std::to_string(playerAvgAnsTime));

	return personalStatistics;
}
