
#include "Parser.h"
// SpeedLog
#include "../../Log/SpeedLog.h"
#include "../tracy-master/Tracy.hpp"
namespace NS_SERIALISER {
	template<typename d_type>
	inline void Parser::ChangeData(std::string d_name, d_type data)
	{

		for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
		{
			for (Value::ConstMemberIterator itr2 = doc[itr->name.GetString()].MemberBegin(); itr2 != doc[itr->name.GetString()].MemberEnd(); ++itr2)
			{
				//See if variable name is the same
				if (d_name == itr2->name.GetString())
				{
					doc[itr->name][itr2->name] = data;

					TracyMessageL("Parser::ChangeData: Data changed");
					SPEEDLOG("Parser::ChangeData: Data changed");
					//std::cout << "Data changed" << std::endl;
					//PrintCurrentData();
					return;

				}
			}
		}
		//If no data change
		TracyMessageL("Parser::ChangeData: Unknown name taken");
		SPEEDLOG("Parser::ChangeData: Unknown name taken");
		//std::cout << "Unknown name taken" << std::endl;
	}
/*
	inline void ChangeData(Value* val, std::string d_name, Value& data)
	{
		Value key(d_name.c_str(), global_alloc);
		//Value _data(data);
		val->GetObject().AddMember(key, data, global_alloc);
	}

	//For object access change
	template<typename d_type>
	inline void ChangeData(Value* val, std::string d_name, d_type data)
	{
		Value key(d_name.c_str(), global_alloc);
		Value _data(data);
		val->GetObject().AddMember(key, _data, global_alloc);
	}*/
	/*
	inline void ChangeDataWithObject(Value* val, std::string name, Value* data)
	{
		Value key(name.c_str(), global_alloc);
		//Value _data(data);
		val->GetObject().AddMember(key, data->GetObject(), global_alloc);
	}*/

	template<>
	inline void Parser::AddData(std::string d_name, Value* val, Document::AllocatorType* _alloc)
	{
		if (_alloc == nullptr)
			_alloc = alloc;

		//Document::AllocatorType& temp_alloc = doc.GetAllocator();
		Value key(d_name.c_str(), *_alloc);

		doc.AddMember(key, *val, *_alloc);
		delete val;
	}
	template<typename d_type>
	inline void Parser::AddData(std::string d_name, d_type data, Document::AllocatorType* _alloc)
	{
		if (_alloc == nullptr)
			_alloc = alloc;

		//Document::AllocatorType& temp_alloc = doc.GetAllocator();
		Value key(d_name.c_str(), *_alloc);
		Value val(data);

		doc.AddMember(key, val, *_alloc);
	}
}
