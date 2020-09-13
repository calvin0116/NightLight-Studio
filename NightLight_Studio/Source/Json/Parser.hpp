//======================= Gonna be in hpp soon =====================================//
template<typename d_type>
inline void Parser::ChangeData(std::string name, d_type data)
{

	for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
	{
		for (Value::ConstMemberIterator itr2 = doc[itr->name.GetString()].MemberBegin(); itr2 != doc[itr->name.GetString()].MemberEnd(); ++itr2)
		{
			//See if variable name is the same
			if (name == itr2->name.GetString())
			{
				doc[itr->name][itr2->name] = data;

				std::cout << "Data changed" << std::endl;
				//PrintCurrentData();
				return;

			}
		}
	}
	//If no data change
	std::cout << "Unknown name taken" << std::endl;
}

//For object access change

template<typename d_type>
inline void Parser::ChangeData(Value* val, std::string name, d_type data)
{
	Document::AllocatorType& alloc = doc.GetAllocator();
	Value key(name.c_str(), alloc);
	Value _data(data);
	val->GetObject().AddMember(key, _data, alloc);
}
/*
template<>
inline void Parser::ChangeData<std::string>(std::string name, std::string data)
{
	for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr)
	{
		for (Value::ConstMemberIterator itr2 = doc[itr->name.GetString()].MemberBegin(); itr2 != doc[itr->name.GetString()].MemberEnd(); ++itr2)
		{
			//See if variable name is the same
			if (name == itr2->name.GetString())
			{
				//Value& nani = itr2->value;
				//Value val(data.c_str());
				//itr2->value;
				//doc[itr->name][itr2->name] = data;
				//doc.AddMember(itr->name, val, *alloc);

				//std::cout << "Data changed" << std::endl;
				//PrintCurrentData();

			}

		}
		return;
	}

	std::cout << "Unknown name taken" << std::endl;
}*/
template<>
inline void Parser::AddData(std::string name, Value* val)
{

	Document::AllocatorType& alloc = doc.GetAllocator();
	Value key(name.c_str(), alloc);

	doc.AddMember(key, *val, alloc);
	delete val;
}
template<typename d_type>
inline void Parser::AddData(std::string name, d_type data)
{

	Document::AllocatorType& alloc = doc.GetAllocator();
	Value key(name.c_str(), alloc);
	Value val(data);

	doc.AddMember(key, val, alloc);
}


