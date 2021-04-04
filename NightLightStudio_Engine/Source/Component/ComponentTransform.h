#pragma once
#include "..\\Math\Vector.h"
#include "..\\Math\Matrix4x4.h"
#include "..\glm\glm.hpp"
#include "..\glm\gtc\type_ptr.hpp"
#include "../../framework.h"
#include "../../ISerializable.h"

#include "LocalString.h"

const std::string S_OBS = "Obstacle";

const std::vector<std::string>TAGNAMES =
{
	"None",
	S_OBS
};

typedef class ComponentTransform  : public ISerializable//: public IComponent
{
public:
	//Name
	//char* name;

	LocalString<256> _entityName;
	int _tag;

	LocalVector<int> _tagNames;

	//For prefab referencing
	bool _isPrefab;
	std::string _prefabToRef;
	int _prefab_Uid;

	//for collision check
	NlMath::Vector3D _nextPos;

	glm::vec3 _position;
	glm::vec3 _rotation; // Euler angles x, y, z
	glm::vec3 _scale;


	glm::vec3 _phyposition;

	ComponentTransform();
	~ComponentTransform();

	glm::mat4 GetModelMatrix();

	//read and write function for initialization from saved files
	void	Read(Value& val, NS_COMPONENT::ComponentManager::ComponentSetManager* g_ecman = G_ECMANAGER) override;
	Value	Write();			
	Value&	Write(Value& val);	// Ovewrite data that has alr have a memory slot
	virtual ComponentTransform* Clone() 
	{ 
		ComponentTransform* newTransform = new ComponentTransform();
		*newTransform = *this;
		return newTransform;
	
	}
} TransformComponent;

//Operator overloading for cout
inline std::ostream& operator<<(std::ostream& os, const ComponentTransform& ct)
{
	os << "Position:" << ct._position.x << "," << ct._position.y << "," << ct._position.z << std::endl;
	os << "Rotation:" << ct._rotation.x << "," << ct._rotation.y << "," << ct._rotation.z << std::endl;
	os << "Scale:" << ct._scale.x << "," << ct._scale.y << "," << ct._scale.z << std::endl;
	return os;
}