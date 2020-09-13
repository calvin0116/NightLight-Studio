/***************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : TransformComponent.cpp
Author : Eric Aw Yuh Liang
DP email : yuhliangeric.a@digipen.edu
Course : GAM200F19-A
Brief Description :
**************************************************************************/
#include "TransformComponent.h"
//#include "CanvasComponent.h"


TransformComponent::TransformComponent()
	: position(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 0.0f),
	rotation(0.0f, 0.0f, 0.0f)
{
	component_name = "Transform";
}


TransformComponent::TransformComponent(float pos_x, float pos_y, float pos_z)
	: position(pos_x, pos_y, pos_z),
	scale(1.0f, 1.0f, 0.0f),
	rotation(0.0f, 0.0f, 0.0f)
{
	component_name = "Transform";
}


TransformComponent::TransformComponent(const TransformComponent& rhs)
	: Component(rhs), position(rhs.position),
	scale(rhs.scale),
	rotation(rhs.rotation)
{
	//rhs.RevertToCopy();

	component_name = "Transform";
	Init();
}


TransformComponent::~TransformComponent()
{
	
	if (copy != nullptr)
	{
		delete reinterpret_cast<TransformComponent*>(copy);
		copy = nullptr;
	}
	//RevertToCopy();
}


void TransformComponent::Init() 
{
  /*
	debug_box_mesh = GraphicsStructureManager::GetInstance()->CreateMesh();
  debug_box_mesh->SetVerticesColour(0.0f, 1.0f, 0.0f, 1.0f);*/
}


void TransformComponent::Update() 
{
	/*
	Mat4Scale(scaling_matrix, scale.x, scale.y, 0);
	Mat4RotDeg(rotation_matrix, rotation.z);
	Mat4Translate(translation_matrix, position.x, position.y, position.z);
	model_matrix = translation_matrix * rotation_matrix * scaling_matrix;
	*/
}


Vec3& TransformComponent::GetPositionRef() //const
{
	return position;
}


Vec3 TransformComponent::GetPosition() //const
{
	return position;
}


Vec3 TransformComponent::GetInitialPosition()
{
	if (copy == nullptr)
		StoreCopy();
	return reinterpret_cast<TransformComponent*>(copy)->GetPosition();
}

void TransformComponent::SetPosition(const Vec3& new_position)
{
	position = new_position;
}


void TransformComponent::SetPosition(float new_x, float new_y)
{
	position.x = new_x;
	position.y = new_y;
}


void TransformComponent::SetPosition(float new_x, float new_y, float new_z)
{
	position.x = new_x;
	position.y = new_y;
	position.z = new_z;
}


void TransformComponent::SetPositionX(float new_x)
{
	position.x = new_x;
}


void TransformComponent::SetPositionY(float new_y)
{
	position.y = new_y;
}


void TransformComponent::SetPositionZ(float new_z)
{
	position.z = new_z;
}


//Vec3 TransformComponent::GetNew_n_Old_pos_Offset(const Vec3 newpos)
//{
//  return newpos - old_pos;
//}

void TransformComponent::Translate(float delta_x, float delta_y, float delta_z)
{
	position.x += delta_x;
	position.y += delta_y;
	position.z += delta_z;
}


void TransformComponent::Translate(Vec3 delta, bool setZ)
{
	position.x += delta.x;
	position.y += delta.y;
	if(setZ)
	position.z += delta.z;
}

void TransformComponent::TranslateRoundToZero()
{
	position.x = std::round(position.x);
	position.y = std::round(position.y);
	position.z = std::round(position.z);
}


void TransformComponent::RotateZAxis(float delta_z)
{
	rotation.z += delta_z;
}


void TransformComponent::Scale(float delta_x, float delta_y)
{
	scale.x += delta_x;
	scale.y += delta_y;
}


Vec3& TransformComponent::GetScaleRef()
{
	return scale;
}

Vec3 TransformComponent::GetScale()
{
	return scale;
}


void TransformComponent::SetScale(const Vec3& new_scale)
{
	scale = new_scale;
}


void TransformComponent::SetScale(float new_x, float new_y, float new_z)
{
  scale.x = new_x;
	scale.y = new_y;
	scale.z = new_z;
}


Vec3& TransformComponent::GetRotation()
{
	return rotation;
}


void TransformComponent::SetRotation(const Vec3& new_rotation)
{
	rotation = new_rotation;
}


void TransformComponent::SetRotation(float new_x, float new_y, float new_z)
{
	rotation.x = new_x;
	rotation.y = new_y;
	rotation.z = new_z;
}


Mat4 TransformComponent::GetTranslationMatrix() const
{
	return translation_matrix;
}


Mat4 TransformComponent::GetRotationMatrix() const
{
	return rotation_matrix;
}


Mat4 TransformComponent::GetModelMatrix() const
{
	return model_matrix;
}


Mat4& TransformComponent::GetModelMatrixRef()
{
	return model_matrix;
}


/*void PrintDebugInfo()
{
	std::cout << "Centre: (" << position.x << ", " << position.y << ")" << std::endl;
	std::cout << "Top left corner: (" << corner_position[0].x << ", " << corner_position[0].y << ")" << std::endl;
	std::cout << "Top right corner: (" << corner_position[1].x << ", " << corner_position[1].y << ")" << std::endl;
	std::cout << "Bottom left corner: (" << corner_position[2].x << ", " << corner_position[2].y << ")" << std::endl;
	std::cout << "Bottom right corner: (" << corner_position[3].x << ", " << corner_position[3].y << ")" << std::endl;
	std::cout << "Edge-normal 0: (" << edge_normal[0].x << ", " << edge_normal[0].y << ")" << std::endl;
	std::cout << "Edge-normal 1: (" << edge_normal[1].x << ", " << edge_normal[1].y << ")" << std::endl;
	std::cout << "Edge-normal 2: (" << edge_normal[2].x << ", " << edge_normal[2].y << ")" << std::endl;
	std::cout << "Edge-normal 3: (" << edge_normal[3].x << ", " << edge_normal[3].y << ")" << std::endl;
}*/

/*
bool TransformComponent::GetDisplayDebugDrawingStatus()
{
	return display_debug_box;
}


void TransformComponent::SetDisplayDebugBoxStatus(bool state)
{
	display_debug_box = state;
}


const Mesh* TransformComponent::GetDebugBoxMesh()
{
	return debug_box_mesh;
}


void TransformComponent::SetDebugBoxLineThickness(float thickness)
{
	debug_box_line_thickness = thickness;
}


float TransformComponent::GetDebugBoxLineThickness() const
{
	return debug_box_line_thickness;
}
*/

TransformComponent* TransformComponent::Clone() 
{
	return new TransformComponent(*this);
}


//this is how u create parameters to load
//and save in a prefab file. 
// It will look something like this
// in the json file:
// "Transform": {
//         "PosX": 0.0,
//         "PosY" : 5.0,
//         "PosZ" : 0.0,
//         "ScaleX" : 1.0,
//         "ScaleY" : 1.0,
//         "ScaleZ" : 0.0
// 		},
/*
nlohmann::json TransformComponent::Serialize() 
{
	nlohmann::json sData;

	sData[GetComponentName()]["PosX"] = position.x;
	sData[GetComponentName()]["PosY"] = position.y;
	sData[GetComponentName()]["PosZ"] = position.z;
	sData[GetComponentName()]["ScaleX"] = scale.x;
	sData[GetComponentName()]["ScaleY"] = scale.y;
	sData[GetComponentName()]["ScaleZ"] = scale.z;
	sData[GetComponentName()]["Rotate"] = rotation.z;

	return sData;
}


//This is how you deserialize your component parameters
//Look at serialize right on top for more info on how to
//Create your own parameters
void TransformComponent::Deserialize(nlohmann::json prefab) 
{
	nlohmann::json obj = prefab[GetComponentName()];

	if (CheckJsonVarExist(obj, "PosX") &&
		CheckJsonVarExist(obj, "PosY") &&
		CheckJsonVarExist(obj, "PosZ"))
		SetPosition(obj["PosX"],obj["PosY"],obj["PosZ"]);
	
	if (CheckJsonVarExist(obj, "ScaleX") &&
		CheckJsonVarExist(obj, "ScaleY") &&
		CheckJsonVarExist(obj, "ScaleZ"))
		SetScale(obj["ScaleX"],obj["ScaleY"],obj["ScaleZ"]);

	if (CheckJsonVarExist(obj, "Rotate"))
		SetRotation(0, 0, obj["Rotate"]);
	
}

void TransformComponent::DeserializeOverWrite(nlohmann::json level)
{
	nlohmann::json obj = level[GetComponentName()];

	if (CheckJsonVarExist(obj, "PosX") &&
		CheckJsonVarExist(obj, "PosY") &&
		CheckJsonVarExist(obj, "PosZ"))
		SetPosition(obj["PosX"], obj["PosY"], obj["PosZ"]);

	if (CheckJsonVarExist(obj, "ScaleX") &&
		CheckJsonVarExist(obj, "ScaleY") &&
		CheckJsonVarExist(obj, "ScaleZ"))
		SetScale(obj["ScaleX"], obj["ScaleY"], obj["ScaleZ"]);


	if (CheckJsonVarExist(obj, "Rotate"))
		SetRotation(0, 0, obj["Rotate"]);
}
*/