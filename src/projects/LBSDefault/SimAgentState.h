#pragma once

class SimAgenState
{
	int agentState;
	static SimAgenState* s_instance;
	SimAgenState(int as = 0)
	{
		agentState = as;
	}
public:
	int get_value()
	{
		return agentState;
	}
	void set_value(int v)
	{
		agentState = v;
	}
	static SimAgenState* getInstance()
	{
		if (!s_instance)
			s_instance = new SimAgenState;
		return s_instance;
	}

};
