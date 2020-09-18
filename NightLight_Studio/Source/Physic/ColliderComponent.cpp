/***************************************************************************
Brief Description :
		A ColliderComponent for the entities
**************************************************************************/
#include "ColliderComponent.h"
#include "TriggerComponent.h"
#include "TransformComponent.h"
/******************************************************************************/
/*!
	Constructor
*/
/******************************************************************************/
ColliderComponent::ColliderComponent() :
	collider_type(ColliderType::AABB),
	collided(false),
	prev_collided(false),
	obj_type(CollisionType::NONE)
	/*,
	display_debug_box(false),
	debug_box_line_thickness(2.0)
	*/
{
	SetComponentName("Collider");
}


ColliderComponent::ColliderComponent(const ColliderComponent& rhs) :
	Component(rhs),
	collider_type(rhs.collider_type),
	collided(rhs.collided),
	prev_collided(rhs.prev_collided),
	obj_type(rhs.obj_type),
	/*
	display_debug_box(rhs.display_debug_box),
	debug_box_line_thickness(rhs.debug_box_line_thickness),
	*/
	position(rhs.position),
	scale(rhs.scale),
	rotation(rhs.rotation)

{
	SetComponentName("Collider");
	Init();
}


/******************************************************************************/
/*!
	Destructor
*/
/******************************************************************************/
ColliderComponent::~ColliderComponent()
{
}
/******************************************************************************/
/*!
	Init Function
*/
/******************************************************************************/
void ColliderComponent::Init()
{
	/*
	debug_box_mesh = GraphicsStructureManager::GetInstance()->CreateMesh();
	debug_box_mesh->SetVerticesColour(0.0f, 0.0f, 1.0f, 1.0f);
	*/
}

/******************************************************************************/
/*!
	Update Function
*/
/******************************************************************************/
void ColliderComponent::Update()
{

	// Check for active status of the object
	if (!GetOwner()->GetActiveStatus())
		return;

	// Used for on Trigger and Collision Checks
	if (isTrigger == true)
	{
		if (collided == true)
		{
			if (prev_collided == false)
			{
	 			OnTriggerEnter();
				prev_collided = true;
			}
			else
				OnTriggerStay();
		}
		else
		{
			if (prev_collided == true)
			{
				OnTriggerExit();
				prev_collided = false;
			}
		}
	}

	// Used for check with the transform being changed that causes the collider box to go infinity
	/*
	if (scale.x > 10000)
	{
		LOG_INFO("Scale x went to infinity");
		scale.x = 1;
		std::cout << GetOwner()->GetComponent<TransformComponent>().GetScale().x << std::endl;
	}
	if (scale.y > 10000)
	{
		LOG_INFO("Scale y went to infinity");
		scale.y = 1;
	}
	
	//ComputeCornerPositions();
	//ComputeEdgeNormals();

	// Set colour of Debug box mesh, based on collision status
	collided ? debug_box_mesh->SetVerticesColour(1.0f, 0.0f, 0.0f, 1.0f) : debug_box_mesh->SetVerticesColour(0.0f, 0.0f, 1.0f, 1.0f);
	*/
}

/******************************************************************************/
/*!
	Getter for the collider box position
*/
/******************************************************************************/
Vec3 ColliderComponent::GetPosition()
{
	return position + GetOwner()->GetComponent<TransformComponent>().GetPosition();
}
/******************************************************************************/
/*!
	Getter for the collider box position reference
*/
/******************************************************************************/
Vec3& ColliderComponent::GetPositionReference() //const
{
	UpdateModelMatrix();
	return position;
}
/******************************************************************************/
/*!
	Getter for the collider box scale reference
*/
/******************************************************************************/
Vec3& ColliderComponent::GetScaleReference()
{
	//std::cout << "Scale : " << scale << std::endl;
	UpdateModelMatrix();
	return scale;
}
/******************************************************************************/
/*!
	Setter for Offset position of the collider box to the transform position
*/
/******************************************************************************/
void ColliderComponent::SetPosition(Vec3 new_position)
{
	position = new_position;
	UpdateModelMatrix();
}
/******************************************************************************/
/*!
	Setter for Offset scale of the collider box to the transform position
*/
/******************************************************************************/
void ColliderComponent::SetScale(Vec3 new_scale)
{
	scale = new_scale;
	UpdateModelMatrix();
}
/******************************************************************************/
/*!
	Getter for scale of the collider box
*/
/******************************************************************************/
Vec3 ColliderComponent::GetScale()
{
	return GetOwner()->GetComponent<TransformComponent>().GetScale() * scale;
}


Vec3 ColliderComponent::GetOffSetPos()
{
	return position;
}


Vec3 ColliderComponent::GetOffSetScal()
{
	return scale;
}


Mat4 ColliderComponent::GetModelMatrix() const
{
	return model_matrix;
}


void ColliderComponent::SetFollowTransformStatus(bool status)
{
	follow_transform = status;
}
/******************************************************************************/
/*!
	Computing the 4 corners of the collider box
*/
/******************************************************************************/
void ColliderComponent::ComputeCornerPositions()
{
	// Getting all corners for the collider Used for OBB checks
	for (uint8_t i = 0; i < 4; ++i)
	{
		switch (i)
		{
		case 0:
			corner_position[i] = Vec3(-(scale.x / 2), (scale.y / 2), position.z);
			break;

		case 1:
			corner_position[i] = Vec3((scale.x / 2), (scale.y / 2), position.z);
			break;

		case 2:
			corner_position[i] = Vec3((scale.x / 2), -(scale.y / 2), position.z);
			break;

		case 3:
			corner_position[i] = Vec3(-(scale.x / 2), -(scale.y / 2), position.z);
			break;
		}
		/*
		Mat4Translate(translation_matrix_corner, corner_position[i].x, corner_position[i].y, 0);
		Mat4RotDeg(rotation_matrix, rotation.z);
		Mat4Translate(translation_matrix, position.x, position.y, 0);
		Mat4 model_matrix_corner = translation_matrix * rotation_matrix * translation_matrix_corner;
		corner_position[i] = Vec3(model_matrix_corner.m[3], model_matrix_corner.m[7], corner_position[i].z);
		*/
	}
}

/******************************************************************************/
/*!
	Compute the edge normal of the collider box
*/
/******************************************************************************/
void ColliderComponent::ComputeEdgeNormals()
{
	for (uint8_t i = 0; i < 4; ++i)
	{
		Vec3 edge_vector = ((i < 3) ? (corner_position[i + 1] - corner_position[i]) :
			(corner_position[0] - corner_position[i]));

		edge_normal[i] = Vec3(-edge_vector.y, edge_vector.x, position.z);
	}
}

/******************************************************************************/
/*!
	Getter for corner position
*/
/******************************************************************************/
Vec3 ColliderComponent::GetCornerPosition(CORNER_TYPE corner)
{
	return corner_position[(int)corner];
}

/******************************************************************************/
/*!
	Getter for edge normal
*/
/******************************************************************************/
Vec3 ColliderComponent::GetEdgeNormal(unsigned int index)
{
	return edge_normal[index];
}

/*
TransformComponent* ColliderComponent::GetTransform()
{
	return &GetOwner()->GetComponent<TransformComponent>();
}*/

/******************************************************************************/
/*!
	Getter for Collision Type example Circle-Cirlce, ABB, OBB
*/
/******************************************************************************/
ColliderComponent::ColliderType ColliderComponent::GetColliderType()
{
	return collider_type;
}
/******************************************************************************/
/*!
	Setter for Collision Type example Circle-Cirlce, ABB, OBB
*/
/******************************************************************************/
void ColliderComponent::setCollisionType(CollisionType type)
{
	obj_type = type;
}


ColliderComponent::CollisionType& ColliderComponent::GetType()
{
	return obj_type;
}

/******************************************************************************/
/*!
	Getter Colliderable
*/
/******************************************************************************/
bool ColliderComponent::GetCollisionStatus()
{
	return collided;
}
/******************************************************************************/
/*!
	Setter Colliderable
*/
/******************************************************************************/
void ColliderComponent::SetCollisionStatus(bool collision_status)
{
	collided = collision_status;
}

/******************************************************************************/
/*!
	Getter for Collider enable
*/
/******************************************************************************/
bool& ColliderComponent::GetEnable()
{
	return enabled;
}

/******************************************************************************/
/*!
	Setter for Collider enable
*/
/******************************************************************************/
void ColliderComponent::SetEnable(bool _enabled)
{
	enabled = _enabled;
}

/******************************************************************************/
/*!
	Getter for trigger enable
*/
/******************************************************************************/
bool& ColliderComponent::GetTrigger()
{
	return isTrigger;
}

/******************************************************************************/
/*!
	Setter for trigger enable
*/
/******************************************************************************/
void ColliderComponent::SetTrigger(bool _trigger)
{
	isTrigger = _trigger;
}

void ColliderComponent::UpdateModelMatrix()
{
	/*
	// Transform the Collision Box from the Transform Component
	Mat4Scale(scaling_matrix,
		GetOwner()->GetComponent<TransformComponent>().GetScale().x * scale.x,
		GetOwner()->GetComponent<TransformComponent>().GetScale().y * scale.y, 0);

	Mat4RotDeg(rotation_matrix, rotation.z);

	Mat4Translate(translation_matrix,
		position.x + GetOwner()->GetComponent<TransformComponent>().GetPosition().x,
		position.y + GetOwner()->GetComponent<TransformComponent>().GetPosition().y,
		position.z + GetOwner()->GetComponent<TransformComponent>().GetPosition().z);

	model_matrix = translation_matrix * rotation_matrix * scaling_matrix;
	*/
}

/*
bool ColliderComponent::GetDisplayDebugDrawingStatus()
{
	return display_debug_box;
}


void ColliderComponent::SetDisplayDebugBoxStatus(bool state)
{
	display_debug_box = state;
}


const Mesh* ColliderComponent::GetDebugBoxMesh()
{
	return debug_box_mesh;
}


void ColliderComponent::SetDebugBoxLineThickness(float thickness)
{
	debug_box_line_thickness = thickness;
}


float ColliderComponent::GetDebugBoxLineThickness() const
{
	return debug_box_line_thickness;
}
*/
/******************************************************************************/
/*!
	Serialization
*/
/******************************************************************************/
/*
nlohmann::json ColliderComponent::Serialize()
{
	nlohmann::json sData;
	sData[GetComponentName()]["Collider_Type"] = collider_type;
	sData[GetComponentName()]["Collider_Enable"] = enabled;
	sData[GetComponentName()]["Collider_Trigger"] = isTrigger;
	// Position
	sData[GetComponentName()]["Pos_x"] = GetOffSetPos().x;
	sData[GetComponentName()]["Pos_y"] = GetOffSetPos().y;
	sData[GetComponentName()]["Pos_z"] = GetOffSetPos().z;
	// Scale
	sData[GetComponentName()]["Scale_x"] = GetOffSetScal().x;
	sData[GetComponentName()]["Scale_y"] = GetOffSetScal().y;
	sData[GetComponentName()]["Scale_z"] = GetOffSetScal().z;
	return sData;
}
*/
/******************************************************************************/
/*!
	Deserialization
*/
/******************************************************************************/
/*
void ColliderComponent::Deserialize(nlohmann::json prefab)
{
	nlohmann::json& obj = prefab["Collider"];

	if (CheckJsonVarExist(obj, "Collider_Type"))
		setCollisionType(obj["Collider_Type"]);

	if (CheckJsonVarExist(obj, "Collider_Enable"))
		SetEnable(obj["Collider_Enable"]);

	if (CheckJsonVarExist(obj, "Collider_Trigger"))
		SetTrigger(obj["Collider_Trigger"]);
	
	if (CheckJsonVarExist(obj, "Pos_x") &&
		CheckJsonVarExist(obj, "Pos_y") &&
		CheckJsonVarExist(obj, "Pos_z"))
		SetPosition(Vec3(obj["Pos_x"] ,obj["Pos_y"],obj["Pos_z"]));


	if (CheckJsonVarExist(obj, "Scale_x") &&
		CheckJsonVarExist(obj, "Scale_y") &&
		CheckJsonVarExist(obj, "Scale_z"))
		SetScale(Vec3(obj["Scale_x"],obj["Scale_y"],obj["Scale_z"]));
}

void ColliderComponent::DeserializeOverWrite(nlohmann::json level_obj)
{
	nlohmann::json& obj = level_obj["Collider"];

	if (CheckJsonVarExist(obj, "Collider_Type"))
		setCollisionType(obj["Collider_Type"]);

	if (CheckJsonVarExist(obj, "Collider_Enable"))
		SetEnable(obj["Collider_Enable"]);

	if (CheckJsonVarExist(obj, "Collider_Trigger"))
		SetTrigger(obj["Collider_Trigger"]);

	if (CheckJsonVarExist(obj, "Pos_x") &&
		CheckJsonVarExist(obj, "Pos_y") &&
		CheckJsonVarExist(obj, "Pos_z"))
		SetPosition(Vec3(obj["Pos_x"],obj["Pos_y"],obj["Pos_z"]));

	if (CheckJsonVarExist(obj, "Scale_x") &&
		CheckJsonVarExist(obj, "Scale_y") &&
		CheckJsonVarExist(obj, "Scale_z"))
		SetScale(Vec3(obj["Scale_x"],obj["Scale_y"],obj["Scale_z"]));
}
*/


/******************************************************************************/
/*!
	Clone function
*/
/******************************************************************************/
ColliderComponent* ColliderComponent::Clone()
{
	return new ColliderComponent(*this);
}

void ColliderComponent::OnTriggerEnter()
{
	if(GetOwner()->HasComponent<TriggerComponent>())
		GetOwner()->GetComponent<TriggerComponent>().OnTriggerEnter();
}

void ColliderComponent::OnTriggerExit()
{
	if (GetOwner()->HasComponent<TriggerComponent>())
		GetOwner()->GetComponent<TriggerComponent>().OnTriggerExit();
}

void ColliderComponent::OnTriggerStay()
{
	if (GetOwner()->HasComponent<TriggerComponent>())
		GetOwner()->GetComponent<TriggerComponent>().OnTriggerStay();
}

void ColliderComponent::Translate(Vec3 delta)
{
	position.x += delta.x;
	position.y += delta.y;
	position.z += delta.z;
}