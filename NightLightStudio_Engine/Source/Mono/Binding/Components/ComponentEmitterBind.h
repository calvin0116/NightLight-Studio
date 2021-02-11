#pragma once
#include "../../../Component/ComponentEmitter.h"
#include <mono/metadata/object.h>

namespace ComponentEmitterBind
{
  bool get_isActive_Internal(EmitterComponent* emt);
  void set_isActive_Internal(EmitterComponent* emt, bool val);

  int get_EmitterID_Internal(EmitterComponent* emt);
  void set_EmitterID_Internal(EmitterComponent* emt, int val);

  void cs_AddTexture_Internal(EmitterComponent* emt, MonoString* val);
  void cs_Play_Internal(EmitterComponent* emt);
  void cs_Stop_Internal(EmitterComponent* emt);

  int get_ShapeType_Internal(EmitterComponent* emt);
  void set_ShapeType_Internal(EmitterComponent* emt, int val);

  float get_Duration_Internal(EmitterComponent* emt);
  void set_Duration_Internal(EmitterComponent* emt, float val);

  unsigned get_EmitRate_Internal(EmitterComponent* emt);
  void set_EmitRate_Internal(EmitterComponent* emt, unsigned val);

  unsigned get_MaxParticle_Internal(EmitterComponent* emt);
  void set_MaxParticle_Internal(EmitterComponent* emt, unsigned val);

  float get_BurstRate_Internal(EmitterComponent* emt);
  void set_BurstRate_Internal(EmitterComponent* emt, float val);

  unsigned get_BurstAmount_Internal(EmitterComponent* emt);
  void set_BurstAmount_Internal(EmitterComponent* emt, unsigned val);

  float get_Angle_Internal(EmitterComponent* emt);
  void set_Angle_Internal(EmitterComponent* emt, float val);

  float get_Radius_Internal(EmitterComponent* emt);
  void set_Radius_Internal(EmitterComponent* emt, float val);

  // Random size settings
  bool get_RandomizeSize_Internal(EmitterComponent* emt);
  void set_RandomizeSize_Internal(EmitterComponent* emt, bool val);

  float get_MinParticleSize_Internal(EmitterComponent* emt);
  void set_MinParticleSize_Internal(EmitterComponent* emt, float val);

  float get_MaxParticleSize_Internal(EmitterComponent* emt);
  void set_MaxParticleSize_Internal(EmitterComponent* emt, float val);

  // Random speed settings
  bool get_RandomizeSpeed_Internal(EmitterComponent* emt);
  void set_RandomizeSpeed_Internal(EmitterComponent* emt, bool val);

  float get_MinParticleSpeed_Internal(EmitterComponent* emt);
  void set_MinParticleSpeed_Internal(EmitterComponent* emt, float val);

  float get_MaxParticleSpeed_Internal(EmitterComponent* emt);
  void set_MaxParticleSpeed_Internal(EmitterComponent* emt, float val);

  // Random lifetime settings
  bool get_RandomizeLife_Internal(EmitterComponent* emt);
  void set_RandomizeLife_Internal(EmitterComponent* emt, bool val);

  float get_MinParticleLife_Internal(EmitterComponent* emt);
  void set_MinParticleLife_Internal(EmitterComponent* emt, float val);

  float get_MaxParticleLife_Internal(EmitterComponent* emt);
  void set_MaxParticleLife_Internal(EmitterComponent* emt, float val);

  bool get_Prewarm_Internal(EmitterComponent* emt);
  void set_Prewarm_Internal(EmitterComponent* emt, bool val);

  bool get_Burst_Internal(EmitterComponent* emt);
  void set_Burst_Internal(EmitterComponent* emt, bool val);

  bool get_Loop_Internal(EmitterComponent* emt);
  void set_Loop_Internal(EmitterComponent* emt, bool val);

  bool get_Follow_Internal(EmitterComponent* emt);
  void set_Follow_Internal(EmitterComponent* emt, bool val);

  bool get_Fade_Internal(EmitterComponent* emt);
  void set_Fade_Internal(EmitterComponent* emt, bool val);

  bool get_VelocityOverTime_Internal(EmitterComponent* emt);
  void set_VelocityOverTime_Internal(EmitterComponent* emt, bool val);

  bool get_SizeOverTime_Internal(EmitterComponent* emt);
  void set_SizeOverTime_Internal(EmitterComponent* emt, bool val);

  bool get_SpeedOverTime_Internal(EmitterComponent* emt);
  void set_SpeedOverTime_Internal(EmitterComponent* emt, bool val);

  bool get_ColourOverTime_Internal(EmitterComponent* emt);
  void set_ColourOverTime_Internal(EmitterComponent* emt, bool val);

  MonoObject* get_ColourA_Internal(EmitterComponent* emt);
  void set_ColourA_Internal(EmitterComponent* emt, MonoObject* val);

  MonoObject* get_ColourB_Internal(EmitterComponent* emt);
  void set_ColourB_Internal(EmitterComponent* emt, MonoObject* val);

  MonoObject* get_ColourStart_Internal(EmitterComponent* emt);
  void set_ColourEnd_Internal(EmitterComponent* emt, MonoObject* val);

  MonoObject* get_Velocity_Internal(EmitterComponent* emt);
  void set_Velocity_Internal(EmitterComponent* emt, MonoObject* val);

  void BindComponentEmitter();
}
