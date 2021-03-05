#include "ComponentTransform.h"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/quaternion.hpp"

#include "Components.h"
#include "../Core/TagHandler.h"
//#include "../IO/Json/Parser.h"


ComponentTransform::ComponentTransform() : _tag(0),
	_isPrefab{false},
	_prefabToRef{""},
	_prefab_Uid{-1},
	_nextPos(0), _position{ 0.0f, 0.0f, 0.0f },
	_rotation{ 0.0f, 0.0f, 0.0f }, _scale{ 1.0f, 1.0f, 1.0f },
	_phyposition{0.0f, 0.0f, 0.0f}
{
	strcpy_s(ser_name, "TransformComponent");
}

ComponentTransform::~ComponentTransform()
{

}

glm::mat4 ComponentTransform::GetModelMatrix()
{
	glm::mat4 Translate = glm::translate(glm::mat4(1.f), _position);
	
	// Perform rotation using Quarternions
	glm::quat Quaternion(glm::radians(_rotation));
	glm::mat4 Rotate = glm::mat4_cast(Quaternion);

	glm::mat4 Scale = glm::scale(glm::mat4(1.f), _scale);

	return (Translate * Rotate * Scale);
}

void ComponentTransform::Read(Value& val)
{
	for (Value::ConstMemberIterator itr = val.MemberBegin(); itr != val.MemberEnd(); ++itr)
	{
		if (itr->name == "EntityName")
		{
			_entityName = itr->value.GetString();
		}
		else if (itr->name == "Tag")
		{
			_tag = itr->value.GetInt();
			TAG_HANDLER->InsertTagToUsed(_tag);
		}
		else if (itr->name == "Position")
		{
			auto pos = itr->value.GetArray();

			_position.x = pos[0].GetFloat();
			_position.y = pos[1].GetFloat();
			_position.z = pos[2].GetFloat();
		}
		else if (itr->name == "Scale")
		{
			auto scale = itr->value.GetArray();

			_scale.x = scale[0].GetFloat();
			_scale.y = scale[1].GetFloat();
			_scale.z = scale[2].GetFloat();
		}
		else if (itr->name == "Rotate")
		{
			auto rotate = itr->value.GetArray();

			_rotation.x = rotate[0].GetFloat();
			_rotation.y = rotate[1].GetFloat();
			_rotation.z = rotate[2].GetFloat();
		}
		else if (itr->name == "tagNames")
		{
			auto tn_list_val = itr->value.GetArray();
			if (_tagNames.size() == 0)
				for (unsigned i = 0; i < tn_list_val.Size(); ++i)
					_tagNames.push_back(tn_list_val[i].GetInt());
			else
				for (unsigned i = 0; i < tn_list_val.Size(); ++i)
					_tagNames.at(i) = tn_list_val[i].GetInt();

			if (_tagNames.size() == 0)
				_tagNames.push_back(0);
		}
		else if (itr->name == "isPrefab")
		{
			_isPrefab = itr->value.GetBool();
		}
	}


	if (val.FindMember("EntityName") == val.MemberEnd())
	{
		TracyMessageL("ComponentTransform::Read: No EntityName data has been found");
		SPEEDLOG("ComponentTransform::Read: No EntityName data has been found");
		//std::cout << "No EntityName data has been found" << std::endl;
	}
	else
		_entityName = val["EntityName"].GetString();

	if (val.FindMember("Tag") == val.MemberEnd())
	{
		TracyMessageL("ComponentTransform::Read: No Tag data has been found");
		SPEEDLOG("ComponentTransform::Read: No Tag data has been found");
		//std::cout << "No Tag data has been found" << std::endl;
	}
	else
	{
		_tag = val["Tag"].GetInt();
		TAG_HANDLER->InsertTagToUsed(_tag);
	}

	//Error checking for json data
	if (val.FindMember("Position") == val.MemberEnd())
	{
		TracyMessageL("ComponentTransform::Read: No Tag data has been found");
		SPEEDLOG("ComponentTransform::Read: No Tag data has been found");
		//std::cout << "No Tag data has been found" << std::endl;
	}
	else
	{
		auto pos = val["Position"].GetArray();

		_position.x = pos[0].GetFloat();
		_position.y = pos[1].GetFloat();
		_position.z = pos[2].GetFloat();
	}
	_phyposition = _position;

	if (val.FindMember("Scale") == val.MemberEnd())
	{
		TracyMessageL("ComponentTransform::Read: No Scale data has been found");
		SPEEDLOG("ComponentTransform::Read: No Scale data has been found");
		//std::cout << "No Scale data has been found" << std::endl;
	}
	else
	{
		auto scale = val["Scale"].GetArray();

		_scale.x = scale[0].GetFloat();
		_scale.y = scale[1].GetFloat();
		_scale.z = scale[2].GetFloat();
	}

	if (val.FindMember("Rotate") == val.MemberEnd())
	{
		TracyMessageL("ComponentTransform::Read: No Rotate data has been found");
		SPEEDLOG("ComponentTransform::Read: No Rotate data has been found");
		//std::cout << "No Rotate data has been found" << std::endl;
	}
	else
	{
		auto rotate = val["Rotate"].GetArray();

		_rotation.x = rotate[0].GetFloat();
		_rotation.y = rotate[1].GetFloat();
		_rotation.z = rotate[2].GetFloat();
	}

	if (val.FindMember("tagNames") == val.MemberEnd())
	{
		TracyMessageL("ComponentTransform::Read: No tagNames data has been found");
		SPEEDLOG("ComponentTransform::Read: No tagNames data has been found");
		//std::cout << "No tagNames data has been found" << std::endl;
	}
	else
	{
		auto tn_list_val = val["tagNames"].GetArray();
		if (_tagNames.size() == 0)
			for (unsigned i = 0; i < tn_list_val.Size(); ++i)
				_tagNames.push_back(tn_list_val[i].GetInt());
		else
			for (unsigned i = 0; i < tn_list_val.Size(); ++i)
				_tagNames.at(i) = tn_list_val[i].GetInt();
	}
	if(_tagNames.size() == 0)
		_tagNames.push_back(0);



}

Value ComponentTransform::Write()
{
	Value val(rapidjson::kObjectType);

	NS_SERIALISER::ChangeData(&val, "EntityName", rapidjson::StringRef(_entityName.c_str())); // Entity Name
  
	NS_SERIALISER::ChangeData(&val, "Tag", _tag);


  Value tn_list_val(rapidjson::kArrayType);
  for (int& tn : _tagNames)
	  tn_list_val.PushBack(tn, global_alloc);
  NS_SERIALISER::ChangeData(&val, "tagNames", tn_list_val);

	Value position(rapidjson::kArrayType);
	position.PushBack(_position.x, global_alloc);
	position.PushBack(_position.y, global_alloc);
	position.PushBack(_position.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Position", position);

	Value scale(rapidjson::kArrayType);
	scale.PushBack(_scale.x, global_alloc);
	scale.PushBack(_scale.y, global_alloc);
	scale.PushBack(_scale.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Scale", scale);

	Value rotation(rapidjson::kArrayType);
	rotation.PushBack(_rotation.x, global_alloc);
	rotation.PushBack(_rotation.y, global_alloc);
	rotation.PushBack(_rotation.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Rotate", rotation);

	return val;
}
//Testing only: Ignore for now
Value& ComponentTransform::Write(Value& val)
{

	NS_SERIALISER::ChangeData(&val, "EntityName", rapidjson::StringRef(_entityName.c_str())); // Entity Name
  NS_SERIALISER::ChangeData(&val, "Tag", _tag);

	// TODO: insert return statement here
	Value position(rapidjson::kArrayType);
	position.PushBack(_position.x, global_alloc);
	position.PushBack(_position.y, global_alloc);
	position.PushBack(_position.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Position", position);

	Value scale(rapidjson::kArrayType);
	scale.PushBack(_scale.x, global_alloc);
	scale.PushBack(_scale.y, global_alloc);
	scale.PushBack(_scale.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Scale", scale);

	Value rotation(rapidjson::kArrayType);
	rotation.PushBack(_rotation.x, global_alloc);
	rotation.PushBack(_rotation.y, global_alloc);
	rotation.PushBack(_rotation.z, global_alloc);

	NS_SERIALISER::ChangeData(&val, "Rotate", rotation);

	return val;
}
