#pragma once

#include <vector>

enum class State
{

};

class Rule
{
public:

private:
};

class StateMachine
{
public:
	StateMachine()
	{

	}

	void AddRule(const Rule& rule)
	{
		m_rules.push_back(rule);
	}

	void Push(char ch)
	{

	}



private:
	std::vector<Rule> m_rules;
};
