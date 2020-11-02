#pragma once

#include <string>

template<size_t N>
class LocalString
{

	char stringData[N];

private:
	// helper fn
	bool copyFromString(const std::string& rhs)
	{
		memcpy(stringData, rhs.c_str(), N - 1);
		stringData[N - 1] = 0;
		return rhs.size() > (N - 1);
	}
	void copyFromLocalString(const LocalString& rhs)
	{
		memcpy(stringData, rhs.stringData, N - 1);
		stringData[N - 1] = 0;
	}
public:

	// default ctor
	LocalString()
	{
		memset(stringData, 0, N);
	}
	
	// init from std::string ctor
	LocalString(std::string& rhs)
	{
		memset(stringData, 0, N);
		copyFromString(rhs);
	}

	// copy ctor
	LocalString(LocalString& rhs)
	{
		copyFromLocalString(rhs);
	}

	// move ctor
	LocalString(LocalString&& rhs)
	{
		copyFromLocalString(rhs);
	}

	// ass op
	void operator=(LocalString& rhs)
	{
		copyFromLocalString(rhs);
	}

	// move op
	void operator=(LocalString&& rhs)
	{
		copyFromLocalString(rhs);
	}

	// = std::String operator
	void operator=(std::string& rhs)
	{
		copyFromString(rhs);
	}

	// = char* operator
	void operator=(const char* rhs)
	{
		std::string str(rhs);
		copyFromString(str);
	}

	// to c_string
	char* c_str()
	{
		return stringData;
	}

	// returns string
	std::string toString()
	{
		return std::string(stringData);
	}




};