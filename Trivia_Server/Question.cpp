#include "Question.h"

Question::Question(const string& question, const vector<string>& wrongAnswers, const string& correctAnswer) :
	m_question(question), m_wrongAnswers(wrongAnswers), m_correctAnswer(correctAnswer)
{
}

string Question::getQuestion() const
{
	return m_question;
}

vector<string> Question::getWrongAnswers() const
{
	return m_wrongAnswers;
}

string Question::getCorrectAnswer() const
{
	return m_correctAnswer;
}
