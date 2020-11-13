#include "ComponentTransformBind.h"
#include "../../MonoWrapper.h"


namespace ComponentTransformBind
{
  void BindComponentTransform()
  {
    MonoWrapper::BindClassFunction(get_Name_Internal, "get_Name_Internal",
      "Transform");
    MonoWrapper::BindClassFunction(set_Name_Internal, "set_Name_Internal",
      "Transform");

    MonoWrapper::BindClassFunction(get_Position_Internal, "get_Position_Internal",
      "Transform");
    MonoWrapper::BindClassFunction(set_Position_Internal, "set_Position_Internal",
      "Transform");

    MonoWrapper::BindClassFunction(get_Rotation_Internal, "get_Rotation_Internal",
      "Transform");
    MonoWrapper::BindClassFunction(set_Rotation_Internal, "set_Rotation_Internal",
      "Transform");

    MonoWrapper::BindClassFunction(get_Scale_Internal, "get_Scale_Internal",
      "Transform");
    MonoWrapper::BindClassFunction(set_Scale_Internal, "set_Scale_Internal",
      "Transform");
  }
  MonoString* get_Name_Internal(TransformComponent* trans)
  {
    return MonoWrapper::ToMonoString(trans->_entityName.toString());
  }

  void set_Name_Internal(TransformComponent* trans, MonoString* val)
  {
    std::string MyStr(MonoWrapper::ToString(val));
    trans->_entityName = MyStr;
  }

  MonoObject* get_Position_Internal(TransformComponent* trans)
  {
    MonoObject* monoVec = MonoWrapper::ConstructObject("Vector3");
    glm::vec3& vec = trans->_position;
    MonoWrapper::SetObjectFieldValue(monoVec, "X", vec.x);
    MonoWrapper::SetObjectFieldValue(monoVec, "Y", vec.y);
    MonoWrapper::SetObjectFieldValue(monoVec, "Z", vec.z);
    return monoVec;
  }

  void set_Position_Internal(TransformComponent* trans, MonoObject* val)
  {
    glm::vec3& vec = trans->_position;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
  }

  MonoObject* get_Rotation_Internal(TransformComponent* trans)
  {
    MonoObject* monoVec = MonoWrapper::ConstructObject("Vector3");
    glm::vec3& vec = trans->_rotation;
    MonoWrapper::SetObjectFieldValue(monoVec, "X", vec.x);
    MonoWrapper::SetObjectFieldValue(monoVec, "Y", vec.y);
    MonoWrapper::SetObjectFieldValue(monoVec, "Z", vec.z);
    return monoVec;
  }

  void set_Rotation_Internal(TransformComponent* trans, MonoObject* val)
  {
    glm::vec3& vec = trans->_rotation;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
  }

  MonoObject* get_Scale_Internal(TransformComponent* trans)
  {
    MonoObject* monoVec = MonoWrapper::ConstructObject("Vector3");
    glm::vec3& vec = trans->_scale;
    MonoWrapper::SetObjectFieldValue(monoVec, "X", vec.x);
    MonoWrapper::SetObjectFieldValue(monoVec, "Y", vec.y);
    MonoWrapper::SetObjectFieldValue(monoVec, "Z", vec.z);
    return monoVec;
  }

  void set_Scale_Internal(TransformComponent* trans, MonoObject* val)
  {
    glm::vec3& vec = trans->_scale;
    vec.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vec.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vec.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
  }
}
