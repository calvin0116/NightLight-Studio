/***************************************************************************
Brief Description :
			Json Parser with these functionality
				-Save and load from Json file
				-Run time adding and changing of variables
				-Accessing data in Json file
			Work in progress
				-Data check to see if it exist (only work for first layer currently)
				-More improvement maybe needed
**************************************************************************/
#pragma once
#ifndef PARSER_H
#define PARSER_H
#include "../../framework.h"
#include "JsonHeader.h"

//Data type of object to distigush it
enum D_TYPE
{
	D_STRING = 0,
	D_INT,
	D_FLOAT,
	D_BOOL,
    D_ARRAY,
	D_OBJECT,
	D_INVALID
};

class ENGINE_API Parser
{
private:
	//Document stuff
	Document doc;
	Document::AllocatorType* alloc;


	std::string name;
	std::string path;
	std::string filepath;

    //Determine type for json member
    D_TYPE DetermineType(Value::ConstMemberIterator itr);
	//Recursive function for PrintDataList()
	void PrintData(Value::ConstMemberIterator itr, Value& val);
public:
    //Constructer with input of file name to parse from
	Parser(std::string name_, std::string path_);

    //Basic function to load/save data into the file
	void Load();
    void Save();

	//To look at the data in raw form
	void PrintDataList();
	//Just to get back file name if needed
	std::string GetName();

	//Add data into the initial json list
    template<typename d_type>
    void AddData(std::string name, d_type data);

	//For known name change
	template<typename d_type>
	void ChangeData(std::string name, d_type data);
	
	//For object access change
	template<typename d_type>
	void ChangeData(Value* val, std::string name, d_type data);
	//Error checking to see if member is in json
	bool CheckForMember(std::string mem_name);
	//Access by name

	Value& operator[](std::string str)
	{
		return doc[str.c_str()];
	}

	//Access by count
	//WIP
	/*
	Value& operator[](int i)
	{
		//if(i > doc.MemberCount())
			//return doc[]
		//return doc[i];

	}*/
};

#include "Parser.hpp"
#endif // !PARSER_h
