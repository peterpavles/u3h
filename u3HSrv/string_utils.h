#pragma once

#include "common.h"

class String_Utils
{
public:
	static vector<string> split(string str, char delimiter) 
	{
		vector<string> internall;
		stringstream ss(str);
		string tok;

		while (getline(ss, tok, delimiter)) 
		{
			internall.push_back(tok);
		}

		return internall;
	}
};