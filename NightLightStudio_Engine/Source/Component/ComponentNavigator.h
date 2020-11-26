#pragma once
#include "..\\Math\Vector.h"
#include "..\\..\ISerializable.h"
//#include "../Ai/AiManager.h"
#include "../Ai/WayPointManager.h"

typedef class ComponentNavigator : public ISerializable //: public IComponent
{
	LocalVector<WayPoint*> way_point_list;
	LocalVector<Edges*> edge_list;

	LocalVector<WayPoint*> cur_path;		//Decided by Astar
	int cur_wp_index;						//Curent targetted waypoint

public:
	ComponentNavigator()
	{
		strcpy_s(ser_name, "NavigatorComponent");
	}

	virtual void	Read(Value&) { };
	virtual Value	Write() { return Value(rapidjson::kObjectType); };
	virtual Value& Write(Value& val) { return val; };	virtual ISerializable* Clone() { return new ISerializable(); }

	void CleanCurPath()
	{
		cur_path.clear();
		cur_wp_index = 0;
	}

	void SetCurrentPath(LocalVector<WayPoint*> wp_list)
	{
		CleanCurPath();
	}


} NavigatorComponent;