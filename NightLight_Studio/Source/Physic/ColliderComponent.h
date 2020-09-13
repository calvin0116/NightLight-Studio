/***************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : ColliderComponent.h
Author : Yap Junxian (100%)
DP email : junxian.yap@digipen.edu
Course : GAM200F19-A
Brief Description :
		A ColliderComponent for the entities
**************************************************************************/
#pragma once
//#include "ECS.h"
//#include "Vec & Matrix/Vector3D.h"
//#include "Vec & Matrix/Matrix4x4.h"
//#include "GraphicsStructureManager.h"
//#include "TransformComponent.h"
#include "Header.h"
#include "Entity.h"
#include "Matrix4x4.h"

class ColliderComponent : public Component
{
public:
	// Corner variable
	enum class CORNER_TYPE
	{
		TOP_LEFT = 0,
		TOP_RIGHT,
		BOTTOM_LEFT,
		BOTTOM_RIGHT
	};
	// ColliderType
	enum class ColliderType
	{
		NONE,
		CIRCLE,
		AABB,
		OBB
	};

	enum class CollisionType
	{
		NONE,
		PLAYER,
		PLATFORM,
		STATIC,
		DYNAMIC,
		CT_TOTAL
	};

	enum TriggerState
	{
		NONE,
		ENTER,
		STAY,
		EXIT
	};

	// Hotspot checks for platform
	unsigned int flag = 0;
	const unsigned int	COLLISION_LEFT = 0x00000001;	//0001
	const unsigned int	COLLISION_RIGHT = 0x00000002;	//0010
	const unsigned int	COLLISION_TOP = 0x00000004;		//0100
	const unsigned int	COLLISION_BOTTOM = 0x00000008;	//1000

private:
	Vec3 position;
	Vec3 scale;
	Vec3 rotation;

	Vec3 transform_position_offset;

	Mat4 scaling_matrix;
	Mat4 rotation_matrix;
	Mat4 translation_matrix;
	Mat4 model_matrix;

	Mat4 translation_matrix_corner;
	Vec3 corner_position[4];
	Vec3 edge_normal[4];

	ColliderType collider_type;
	CollisionType obj_type;



	// CAN TOGGLE
	bool enabled = true;
	bool isTrigger = false;

	// Collision boolean
	bool collided = false;
	bool prev_collided = false;

	/*
	Mesh* debug_box_mesh;
	bool display_debug_box;
	float debug_box_line_thickness;
	*/
	//collided target
	std::vector<ColliderComponent*> col_targets;

	bool follow_transform = false;

public:

	ColliderComponent* lastCollided = nullptr;

	TriggerState prevState = NONE;
	TriggerState currState = NONE;

	bool flying = false;
	bool campfire = false;
	bool windCollide = false;

	ColliderComponent();
	ColliderComponent(const ColliderComponent& rhs);
	~ColliderComponent();

	void Init() override;
	void Update() override;


	void Translate(Vec3 translation);
	Mat4 GetModelMatrix() const;

	void SetFollowTransformStatus(bool status);

	void ComputeCornerPositions();
	void ComputeEdgeNormals();
	//TransformComponent* GetTransform();

	// Getter and Setter for the ColliderComponent
	Vec3 GetPosition();
	Vec3 GetScale();
	Vec3 GetOffSetPos();
	Vec3 GetOffSetScal();
	Vec3& GetPositionReference();
	void SetPosition(Vec3 new_position);
	void SetScale(Vec3 new_scale);
	Vec3& GetScaleReference();
	Vec3 GetCornerPosition(CORNER_TYPE corner);
	Vec3 GetEdgeNormal(unsigned int index);
	ColliderType GetColliderType();
	void setCollisionType(CollisionType type);
	CollisionType& GetType();
	bool GetCollisionStatus();
	void SetCollisionStatus(bool collision_status);
	bool& GetEnable();
	void SetEnable(bool _collide);
	bool& GetTrigger();
	void SetTrigger(bool _trigger);

	//For matrixes
	void UpdateModelMatrix();
	/*
	bool GetDisplayDebugDrawingStatus();
	void SetDisplayDebugBoxStatus(bool state);
	const Mesh* GetDebugBoxMesh();
	void SetDebugBoxLineThickness(float thickness);
	float GetDebugBoxLineThickness() const;

	nlohmann::json Serialize() override;
	void Deserialize(nlohmann::json prefab) override;
	void DeserializeOverWrite(nlohmann::json level_obj) override;
	*/
	ColliderComponent* Clone() override;
	
	// Different Collision Checks
	virtual void OnTriggerEnter();
	virtual void OnTriggerExit();
	virtual void OnTriggerStay();
};
