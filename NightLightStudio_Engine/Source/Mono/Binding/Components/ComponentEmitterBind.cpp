#include "ComponentEmitterBind.h"
#include "../../MonoWrapper.h"


namespace ComponentEmitterBind
{
  void BindComponentEmitter()
  {
    MonoWrapper::BindClassFunction(get_isActive_Internal, "get_isActive_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_isActive_Internal, "set_isActive_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_EmitterID_Internal, "get_EmitterID_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_EmitterID_Internal, "set_EmitterID_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_ShapeType_Internal, "get_ShapeType_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_ShapeType_Internal, "set_ShapeType_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Duration_Internal, "get_Duration_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Duration_Internal, "set_Duration_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_EmitRate_Internal, "get_EmitRate_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_EmitRate_Internal, "set_EmitRate_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_MaxParticle_Internal, "get_MaxParticle_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_MaxParticle_Internal, "set_MaxParticle_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_BurstRate_Internal, "get_BurstRate_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_BurstRate_Internal, "set_BurstRate_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_BurstAmount_Internal, "get_BurstAmount_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_BurstAmount_Internal, "set_BurstAmount_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Angle_Internal, "get_Angle_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Angle_Internal, "set_Angle_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Radius_Internal, "get_Radius_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Radius_Internal, "set_Radius_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_RandomizeSize_Internal, "get_RandomizeSize_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_RandomizeSize_Internal, "set_RandomizeSize_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_MinParticleSize_Internal, "get_MinParticleSize_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_MinParticleSize_Internal, "set_MinParticleSize_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_MaxParticleSize_Internal, "get_MaxParticleSize_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_MaxParticleSize_Internal, "set_MaxParticleSize_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_RandomizeSpeed_Internal, "get_RandomizeSpeed_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_RandomizeSpeed_Internal, "set_RandomizeSpeed_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_MinParticleSpeed_Internal, "get_MinParticleSpeed_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_MinParticleSpeed_Internal, "set_MinParticleSpeed_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_MinParticleSpeed_Internal, "get_MinParticleSpeed_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_MinParticleSpeed_Internal, "set_MinParticleSpeed_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_MaxParticleSpeed_Internal, "get_MaxParticleSpeed_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_MaxParticleSpeed_Internal, "set_MaxParticleSpeed_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_RandomizeLife_Internal, "get_RandomizeLife_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_RandomizeLife_Internal, "set_RandomizeLife_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_MinParticleLife_Internal, "get_MinParticleLife_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_MinParticleLife_Internal, "set_MinParticleLife_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_MaxParticleLife_Internal, "get_MaxParticleLife_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_MaxParticleLife_Internal, "set_MaxParticleLife_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Prewarm_Internal, "get_Prewarm_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Prewarm_Internal, "set_Prewarm_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Burst_Internal, "get_Burst_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Burst_Internal, "set_Burst_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Loop_Internal, "get_Loop_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Loop_Internal, "set_Loop_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Follow_Internal, "get_Follow_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Follow_Internal, "set_Follow_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Fade_Internal, "get_Fade_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Fade_Internal, "set_Fade_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_VelocityOverTime_Internal, "get_VelocityOverTime_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_VelocityOverTime_Internal, "set_VelocityOverTime_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_SizeOverTime_Internal, "get_SizeOverTime_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_SizeOverTime_Internal, "set_SizeOverTime_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_SpeedOverTime_Internal, "get_SpeedOverTime_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_SpeedOverTime_Internal, "set_SpeedOverTime_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_ColourOverTime_Internal, "get_ColourOverTime_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_ColourOverTime_Internal, "set_ColourOverTime_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_ColourA_Internal, "get_ColourA_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_ColourA_Internal, "set_ColourA_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_ColourB_Internal, "get_ColourB_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_ColourB_Internal, "set_ColourB_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_ColourStart_Internal, "get_ColourStart_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_ColourStart_Internal, "set_ColourStart_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_ColourEnd_Internal, "get_ColourEnd_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_ColourEnd_Internal, "set_ColourEnd_Internal", "Emitter");

    MonoWrapper::BindClassFunction(get_Velocity_Internal, "get_Velocity_Internal", "Emitter");
    MonoWrapper::BindClassFunction(set_Velocity_Internal, "set_Velocity_Internal", "Emitter");

    MonoWrapper::BindClassFunction(cs_AddTexture_Internal, "cs_AddTexture_Internal", "Emitter");
    MonoWrapper::BindClassFunction(cs_Play_Internal, "cs_Play_Internal", "Emitter");
    MonoWrapper::BindClassFunction(cs_Stop_Internal, "cs_Stop_Internal", "Emitter");
  }

  bool get_isActive_Internal(EmitterComponent* emt)
  {
    return emt->_isActive;
  }
  void set_isActive_Internal(EmitterComponent* emt, bool val)
  {
    emt->_isActive = val;
  }

  int get_EmitterID_Internal(EmitterComponent* emt)
  {
    return emt->GetEmitterID();
  }
  void set_EmitterID_Internal(EmitterComponent* emt, int val)
  {
    emt->SetEmitterID(val);
  }

  int get_ShapeType_Internal(EmitterComponent* emt)
  {
    return emt->GetShapeType();
  }
  void set_ShapeType_Internal(EmitterComponent* emt, int val)
  {
    emt->SetShapeType(static_cast<EmitterShapeType>(val));
  }

  float get_Duration_Internal(EmitterComponent* emt)
  {
    return emt->GetDuration();
  }
  void set_Duration_Internal(EmitterComponent* emt, float val)
  {
    emt->SetDuration(val);
  }

  unsigned get_EmitRate_Internal(EmitterComponent* emt)
  {
    return emt->GetEmissionRate();
  }
  void set_EmitRate_Internal(EmitterComponent* emt, unsigned val)
  {
    emt->SetEmissionRate(val);
  }

  unsigned get_MaxParticle_Internal(EmitterComponent* emt)
  {
    return emt->GetMaxParticle();
  }
  void set_MaxParticle_Internal(EmitterComponent* emt, unsigned val)
  {
    emt->SetMaxParticle(val);
  }

  float get_BurstRate_Internal(EmitterComponent* emt)
  {
    return emt->GetBurstRate();
  }
  void set_BurstRate_Internal(EmitterComponent* emt, float val)
  {
    emt->SetBurstRate(val);
  }

  unsigned get_BurstAmount_Internal(EmitterComponent* emt)
  {
    return emt->GetBurstAmount();
  }
  void set_BurstAmount_Internal(EmitterComponent* emt, unsigned val)
  {
    emt->SetBurstAmount(val);
  }

  float get_Angle_Internal(EmitterComponent* emt)
  {
    return emt->GetAngle();
  }
  void set_Angle_Internal(EmitterComponent* emt, float val)
  {
    emt->SetAngle(val);
  }

  float get_Radius_Internal(EmitterComponent* emt)
  {
    return emt->GetRadius();
  }
  void set_Radius_Internal(EmitterComponent* emt, float val)
  {
    emt->SetRadius(val);
  }

  // Random size settings
  bool get_RandomizeSize_Internal(EmitterComponent* emt)
  {
    return emt->GetRandomizeSize();
  }
  void set_RandomizeSize_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetRandomizeSize(val);
  }

  float get_MinParticleSize_Internal(EmitterComponent* emt)
  {
    return emt->GetMinParticleSize();
  }
  void set_MinParticleSize_Internal(EmitterComponent* emt, float val)
  {
    emt->SetMinParticleSize(val);
  }

  float get_MaxParticleSize_Internal(EmitterComponent* emt)
  {
    return emt->GetMaxParticleSize();
  }
  void set_MaxParticleSize_Internal(EmitterComponent* emt, float val)
  {
    emt->SetMaxParticleSize(val);
  }

  // Random speed settings
  bool get_RandomizeSpeed_Internal(EmitterComponent* emt)
  {
    return emt->GetRandomizeSpeed();
  }
  void set_RandomizeSpeed_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetRandomizeSpeed(val);
  }

  float get_MinParticleSpeed_Internal(EmitterComponent* emt)
  {
    return emt->GetMinParticleSpeed();
  }
  void set_MinParticleSpeed_Internal(EmitterComponent* emt, float val)
  {
    emt->SetMinParticleSpeed(val);
  }

  float get_MaxParticleSpeed_Internal(EmitterComponent* emt)
  {
    return emt->GetMaxParticleSpeed();
  }
  void set_MaxParticleSpeed_Internal(EmitterComponent* emt, float val)
  {
    emt->SetMaxParticleSpeed(val);
  }

  // Random lifetime settings
  bool get_RandomizeLife_Internal(EmitterComponent* emt)
  {
    return emt->GetRandomizeLife();
  }
  void set_RandomizeLife_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetRandomizeLife(val);
  }

  float get_MinParticleLife_Internal(EmitterComponent* emt)
  {
    return emt->GetMinParticleLife();
  }
  void set_MinParticleLife_Internal(EmitterComponent* emt, float val)
  {
    emt->SetMinParticleLife(val);
  }

  float get_MaxParticleLife_Internal(EmitterComponent* emt)
  {
    return emt->GetMaxParticleLife();
  }
  void set_MaxParticleLife_Internal(EmitterComponent* emt, float val)
  {
    emt->SetMaxParticleLife(val);
  }

  bool get_Prewarm_Internal(EmitterComponent* emt)
  {
    return emt->GetPrewarm();
  }
  void set_Prewarm_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetPrewarm(val);
  }

  bool get_Burst_Internal(EmitterComponent* emt)
  {
    return emt->GetBurst();
  }
  void set_Burst_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetBurst(val);
  }

  bool get_Loop_Internal(EmitterComponent* emt)
  {
    return emt->GetLoop();
  }
  void set_Loop_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetLoop(val);
  }

  bool get_Follow_Internal(EmitterComponent* emt)
  {
    return emt->GetFollow();
  }
  void set_Follow_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetFollow(val);
  }

  bool get_Fade_Internal(EmitterComponent* emt)
  {
    return emt->GetFade();
  }
  void set_Fade_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetFade(val);
  }

  bool get_VelocityOverTime_Internal(EmitterComponent* emt)
  {
    return emt->GetVelocityOverTime();
  }
  void set_VelocityOverTime_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetVelocityOverTime(val);
  }

  bool get_SizeOverTime_Internal(EmitterComponent* emt)
  {
    return emt->GetSizeOverTime();
  }
  void set_SizeOverTime_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetSizeOverTime(val);
  }

  bool get_SpeedOverTime_Internal(EmitterComponent* emt)
  {
    return emt->GetSpeedOverTime();
  }
  void set_SpeedOverTime_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetSpeedOverTime(val);
  }

  bool get_ColourOverTime_Internal(EmitterComponent* emt)
  {
    return emt->GetColourOverTime();
  }
  void set_ColourOverTime_Internal(EmitterComponent* emt, bool val)
  {
    emt->SetColourOverTime(val);
  }

  // Vec4 for colour
  MonoObject* get_ColourA_Internal(EmitterComponent* emt)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Vector4");
    glm::vec4 col = emt->GetColourA();
    MonoWrapper::SetObjectFieldValue(monoObj, "X", col.x);
    MonoWrapper::SetObjectFieldValue(monoObj, "Y", col.y);
    MonoWrapper::SetObjectFieldValue(monoObj, "Z", col.z);
    MonoWrapper::SetObjectFieldValue(monoObj, "W", col.w);
    return monoObj;
  }
  void set_ColourA_Internal(EmitterComponent* emt, MonoObject* val)
  {
    glm::vec4 col;
    col.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    col.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    col.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
    col.w = MonoWrapper::GetObjectFieldValue<float>(val, "W");
    emt->SetColourA(col);
  }

  MonoObject* get_ColourB_Internal(EmitterComponent* emt)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Vector4");
    glm::vec4 col = emt->GetColourB();
    MonoWrapper::SetObjectFieldValue(monoObj, "X", col.x);
    MonoWrapper::SetObjectFieldValue(monoObj, "Y", col.y);
    MonoWrapper::SetObjectFieldValue(monoObj, "Z", col.z);
    MonoWrapper::SetObjectFieldValue(monoObj, "W", col.w);
    return monoObj;
  }
  void set_ColourB_Internal(EmitterComponent* emt, MonoObject* val)
  {
    glm::vec4 col;
    col.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    col.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    col.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
    col.w = MonoWrapper::GetObjectFieldValue<float>(val, "W");
    emt->SetColourB(col);
  }

  MonoObject* get_ColourStart_Internal(EmitterComponent* emt)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Vector4");
    glm::vec4 col = emt->GetColourStart();
    MonoWrapper::SetObjectFieldValue(monoObj, "X", col.x);
    MonoWrapper::SetObjectFieldValue(monoObj, "Y", col.y);
    MonoWrapper::SetObjectFieldValue(monoObj, "Z", col.z);
    MonoWrapper::SetObjectFieldValue(monoObj, "W", col.w);
    return monoObj;
  }
  void set_ColourStart_Internal(EmitterComponent* emt, MonoObject* val)
  {
    glm::vec4 col;
    col.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    col.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    col.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
    col.w = MonoWrapper::GetObjectFieldValue<float>(val, "W");
    emt->SetColourStart(col);
  }

  MonoObject* get_ColourEnd_Internal(EmitterComponent* emt)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Vector4");
    glm::vec4 col = emt->GetColourEnd();
    MonoWrapper::SetObjectFieldValue(monoObj, "X", col.x);
    MonoWrapper::SetObjectFieldValue(monoObj, "Y", col.y);
    MonoWrapper::SetObjectFieldValue(monoObj, "Z", col.z);
    MonoWrapper::SetObjectFieldValue(monoObj, "W", col.w);
    return monoObj;
  }
  void set_ColourEnd_Internal(EmitterComponent* emt, MonoObject* val)
  {
    glm::vec4 col;
    col.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    col.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    col.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
    col.w = MonoWrapper::GetObjectFieldValue<float>(val, "W");
    emt->SetColourEnd(col);
  }

  // Vec3 for velocity
  MonoObject* get_Velocity_Internal(EmitterComponent* emt)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Vector3");
    glm::vec3 vel = emt->GetVelocity();
    MonoWrapper::SetObjectFieldValue(monoObj, "X", vel.x);
    MonoWrapper::SetObjectFieldValue(monoObj, "Y", vel.y);
    MonoWrapper::SetObjectFieldValue(monoObj, "Z", vel.z);
    return monoObj;
  }
  void set_Velocity_Internal(EmitterComponent* emt, MonoObject* val)
  {
    glm::vec3 vel;
    vel.x = MonoWrapper::GetObjectFieldValue<float>(val, "X");
    vel.y = MonoWrapper::GetObjectFieldValue<float>(val, "Y");
    vel.z = MonoWrapper::GetObjectFieldValue<float>(val, "Z");
    emt->SetVelocity(vel);
  }

  void cs_AddTexture_Internal(EmitterComponent* emt, MonoString* val)
  {
    emt->AddTexture(MonoWrapper::ToString(val));
  }
  void cs_Play_Internal(EmitterComponent* emt)
  {
    emt->Play();
  }
  void cs_Stop_Internal(EmitterComponent* emt)
  {
    emt->Stop();
  }
}
