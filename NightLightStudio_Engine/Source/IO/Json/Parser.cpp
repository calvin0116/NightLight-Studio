#include "..\..\..\framework.h"
#include "Parser.h"
//#include "rapidjson/rapidjson.h"

Parser::Parser(std::string name_, std::string path_)
{
	name = name_;
	path = path_;

	alloc = &doc.GetAllocator();
}

void Parser::Load()
{
	//StringBuffer s;
	filepath = path + name;
		//assert_path + "config.json";

	//Read in the data from the file
	std::ifstream in(filepath, std::ios::binary);

	if (!in)
	{
		cout << "Error: Failed to open file in: " + filepath << endl;
	}
	// dont skip on whitespace
	std::noskipws(in);
	// Read in content
	std::istreambuf_iterator<char> head(in);
	std::istreambuf_iterator<char> tail;
	std::string data(head, tail);

	//std::cout << data << std::endl;
	//Interpret json formated string
	doc.Parse(data.c_str());
	//Loop through the objects
	PrintDataList();

    in.close();
    /*
    filepath = path + "is_prefab.json";
    in.open(filepath, std::ios::binary);

    if (!in)
    {
        cout << "Error: Failed to open file in: " + filepath << endl;
    }

    std::noskipws(in);
    // Read in content
    std::istreambuf_iterator<char> head2(in);
    std::istreambuf_iterator<char> tail2;
    std::string data2(head2, tail2);

    std::cout << data2 << std::endl;
    //Interpret json formated string
    doc.Parse(data2.c_str());
    //Loop through the objects
    PrintDataList();

    in.close();*/
}

void Parser::PrintDataList() 
{	
	cout << "Printing through doc:" << std::endl;
	for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
	{
        PrintData(itr,(Value&)doc);
	}
}



std::string Parser::GetName()
{
	return name;
}

bool Parser::CheckForMember(std::string mem_name)
{
    //Check for first layer
    if (doc.HasMember(mem_name.c_str()))
        return true;

    for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
    {
        for (Value::ConstMemberIterator itr2 = doc[itr->name.GetString()].MemberBegin(); itr2 != doc[itr->name.GetString()].MemberEnd(); ++itr2)
        {
            //See if variable name is the same
            if (mem_name == itr2->name.GetString())
            {
                std::cout << "Member found" << std::endl;
                return true;
            }
        }
    }
    return false;
}

D_TYPE Parser::DetermineType(Value::ConstMemberIterator itr)
{
    /*//Debug type
    static const char* kTypeNames[] =
    { "Null", "False", "True", "Object", "Array", "String", "Number" };
    std::cout << kTypeNames[itr->value.GetType()] << std::endl;
    */
	if (itr->value.IsBool())
	{
		return D_TYPE::D_BOOL;
	}
	else if (itr->value.IsInt())
	{
		return D_TYPE::D_INT;
	}
	else if (itr->value.IsFloat())
	{
		return D_TYPE::D_FLOAT;
	}
	else if (itr->value.IsString())
	{
		return D_TYPE::D_STRING;
	}
    else if (itr->value.IsObject())
    {
        return D_TYPE::D_ARRAY;
    }

	return D_TYPE::D_INVALID;
}

void Parser::PrintData(Value::ConstMemberIterator itr, Value& val)
{
    D_TYPE data_type = DetermineType(itr);

    if (data_type != D_TYPE::D_ARRAY)
        std::cout << "    ";
        
    std::cout << itr->name.GetString() << " : ";

    switch (data_type)
    {
        case D_TYPE::D_BOOL:
            std::cout << itr->value.GetBool() << '\n';
            break;
        case D_TYPE::D_FLOAT:
            std::cout << itr->value.GetFloat() << '\n';
            break;
        case D_TYPE::D_INT:
            std::cout << itr->value.GetFloat() << '\n';
            break;
        case D_TYPE::D_STRING:
            std::cout << itr->value.GetString() << '\n';
            break;
        case D_TYPE::D_ARRAY:
        {
            std::cout << "{\n";
            for (Value::ConstMemberIterator itr2 = val[itr->name.GetString()].MemberBegin(); itr2 != val[itr->name.GetString()].MemberEnd(); ++itr2)
            {
                PrintData(itr2, val[itr->name.GetString()]);
            }
            cout << "}" << endl;
            break;
        }
    }
}



void Parser::Save()
{
	std::ofstream ofs(filepath);
	OStreamWrapper osw(ofs);

	//StringBuffer buffer(doc);
	PrettyWriter<OStreamWrapper> writer(osw);
	//Writer<OStreamWrapper> writer(osw);
	doc.Accept(writer);
}





