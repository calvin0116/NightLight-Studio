#include "ComponentEmitterBind.h"
#include "../../MonoWrapper.h"


namespace ComponentEmitterBind
{
  void BindComponentEmitter()
  {
    // Add stuff here
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
  MonoObject* get_ColourA_Internal(EmitterComponent* emt);
  void set_ColourA_Internal(EmitterComponent* emt, MonoObject* val);

  MonoObject* get_ColourB_Internal(EmitterComponent* emt);
  void set_ColourB_Internal(EmitterComponent* emt, MonoObject* val);

  MonoObject* get_ColourStart_Internal(EmitterComponent* emt);
  void set_ColourEnd_Internal(EmitterComponent* emt, MonoObject* val);

  // Vec3 for velocity
  MonoObject* get_Velocity_Internal(EmitterComponent* emt)
  {
    MonoObject* monoObj = MonoWrapper::ConstructObject("Vector3D");
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
}
