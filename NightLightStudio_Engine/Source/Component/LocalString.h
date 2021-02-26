#pragma once

#include <string>
#define DEF_STR_SIZE 156

template<size_t N = DEF_STR_SIZE>
class LocalString
{
	char stringData[N];

private:
	// helper fn
	bool copyFromString(const std::string& rhs)
	{
		//size_t sz = rhs.size() < N - 1 ? rhs.size() : N - 1;
		//memcpy(stringData, rhs.c_str(), sz);

		//sz = rhs.size() + 1 < N - 1 ? rhs.size() + 1 : N - 1;
		//stringData[sz] = 0; // nani
		//return rhs.size() > (N - 1);

		memcpy(stringData, rhs.c_str(), N - 1);
		stringData[N - 1] = 0; // nani
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
	LocalString(const std::string& rhs)
	{
		memset(stringData, 0, N);
		copyFromString(rhs);
	}

	// copy ctor
	LocalString(const LocalString& rhs)
	{
		copyFromLocalString(rhs);
	}

	// move ctor
	LocalString(const LocalString&& rhs)
	{
		copyFromLocalString(rhs);
	}

	// ass op
	void operator=(const LocalString& rhs)
	{
		copyFromLocalString(rhs);
	}

	// move op
	void operator=(const LocalString&& rhs) noexcept
	{
		copyFromLocalString(rhs);
	}

	// = std::String operator
	void operator=(const std::string& rhs)
	{
		copyFromString(rhs);
	}

	// = std::String mov operator
	void operator=(const std::string&& rhs)
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

	void clear()
	{
		memset(stringData, 0, N);
	}

	bool empty()
	{
		return stringData[0] == 0;
	}

	// implicitly-convert to std::string // std::string str = this;
	operator std::string() const
	{
		return std::string(stringData);
	}

};