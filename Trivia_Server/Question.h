#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

class Question
{
public:
	// Ctor and Dtor
	Question(const string& question, const vector<string>& wrongAnswers, const string& correctAnswer);
	~Question() = default;

	string getQuestion() const;
	vector<string> getWrongAnswers() const;
	string getCorrectAnswer() const;

private:

	string m_question;
	vector<string> m_wrongAnswers;
	string m_correctAnswer;
};
