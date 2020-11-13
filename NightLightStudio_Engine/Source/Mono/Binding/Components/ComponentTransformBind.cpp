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
}
