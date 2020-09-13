/***************************************************************************
Copyright © 2019 DigiPen (Singapore) Corporation, all rights reserved.

Filename : TransformComponent.h
Author : Eric Aw Yuh Liang
DP email : yuhliangeric.a@digipen.edu
Course : GAM200F19-A
Brief Description :
**************************************************************************/
#pragma once
/*
#include "ECS.h"

#include "GraphicsStructureManager.h"
*/

#include "Matrix4x4.h"
#include "Component.h"


class TransformComponent : public Component
{
private:
	Vec3 position;
	Vec3 scale;
	Vec3 rotation;

	Mat4 scaling_matrix;
	Mat4 rotation_matrix;
	Mat4 translation_matrix;
	Mat4 model_matrix;

	/*
	Mesh* debug_box_mesh;
	bool display_debug_box;
	float debug_box_line_thickness = 2.0f;
	*/
	bool model_updated_elsewhere = false;
	Mat4 elsewhere_model_matrix;

public:
	TransformComponent();
	TransformComponent(float pos_x, float pos_y, float pos_z = 0.0f);
	TransformComponent(const TransformComponent& rhs);
	~TransformComponent();

	void Init() override;
	void Update() override;

	Vec3& GetPositionRef();
	Vec3 GetPosition();

	//For initial position
	Vec3 GetInitialPosition();

	void SetPosition(const Vec3& new_position);
	void SetPosition(float new_x, float new_y);
	void SetPosition(float new_x, float new_y, float new_z);
	void SetPositionX(float new_x);
	void SetPositionY(float new_y);
	void SetPositionZ(float new_z);

  //Vec3 GetNew_n_Old_pos_Offset(const Vec3 newpos);

	void Translate(float delta_x, float delta_y, float delta_z = 0.0f);
	void Translate(Vec3 delta, bool setZ = true);

	void TranslateRoundToZero();

	void RotateZAxis(float delta_z);

	void Scale(float delta_x, float delta_y);

	Vec3& GetScaleRef();
	Vec3 GetScale();

	void SetScale(const Vec3& new_scale);
	void SetScale(float new_x, float new_y, float new_z);

	Vec3& GetRotation();

	void SetRotation(const Vec3& new_rotation);
	void SetRotation(float new_x, float new_y, float new_z);

	Mat4 GetTranslationMatrix() const;

	Mat4 GetRotationMatrix() const;

	Mat4 GetModelMatrix() const;

	Mat4& GetModelMatrixRef();
	/*
	bool GetDisplayDebugDrawingStatus();
	void SetDisplayDebugBoxStatus(bool state);
	const Mesh* GetDebugBoxMesh();
	void SetDebugBoxLineThickness(float thickness);
	float GetDebugBoxLineThickness() const;

	nlohmann::json Serialize() override;
	void Deserialize(nlohmann::json prefab) override;
	void DeserializeOverWrite(nlohmann::json obj) override;
	*/
	TransformComponent* Clone() override;
	void RevertToCopy() override
	{
		if (copy != nullptr)
		{

			TransformComponent* temp = reinterpret_cast<TransformComponent*>(copy);
			*this = *temp;
			
			delete temp;
			copy = nullptr;
		}
	}
};

