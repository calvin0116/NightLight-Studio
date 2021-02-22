
#include "ComponentResolver.h"

#include "..//Component/Components.h"



template<>
int typeResolver<ComponentTransform>()
{
	return ComponentTransform_ind;
}
template<>
void setTypeResolver<ComponentTransform>(int set)
{
	ComponentTransform_ind = set;
}


template<>
int typeResolver<ComponentCollider>()
{
	return ComponentCollider_ind;
}
template<>
void setTypeResolver<ComponentCollider>(int set)
{
	ComponentCollider_ind = set;
}


template<>
int typeResolver<ComponentRigidBody>()
{
	return ComponentRigidBody_ind;
}
template<>
void setTypeResolver<ComponentRigidBody>(int set)
{
	ComponentRigidBody_ind = set;
}


template<>
int typeResolver<ComponentGraphics>()
{
	return ComponentGraphics_ind;
}
template<>
void setTypeResolver<ComponentGraphics>(int set)
{
	ComponentGraphics_ind = set;
}


template<>
int typeResolver<ComponentAudio>()
{
	return ComponentAudio_ind;
}
template<>
void setTypeResolver<ComponentAudio>(int set)
{
	ComponentAudio_ind = set;
}


template<>
int typeResolver<ComponentLoadAudio>()
{
	return ComponentLoadAudio_ind;
}
template<>
void setTypeResolver<ComponentLoadAudio>(int set)
{
	ComponentLoadAudio_ind = set;
}


template<>
int typeResolver<ComponentScript>()
{
	return ComponentScript_ind;
}
template<>
void setTypeResolver<ComponentScript>(int set)
{
	ComponentScript_ind = set;
}


template<>
int typeResolver<ComponentLight>()
{
	return ComponentLight_ind;
}
template<>
void setTypeResolver<ComponentLight>(int set)
{
	ComponentLight_ind = set;
}


template<>
int typeResolver<ComponentCanvas>()
{
	return ComponentCanvas_ind;
}
template<>
void setTypeResolver<ComponentCanvas>(int set)
{
	ComponentCanvas_ind = set;
}


template<>
int typeResolver<ComponentAnimation>()
{
	return ComponentAnimation_ind;
}
template<>
void setTypeResolver<ComponentAnimation>(int set)
{
	ComponentAnimation_ind = set;
}


template<>
int typeResolver<ComponentEmitter>()
{
	return ComponentEmitter_ind;
}
template<>
void setTypeResolver<ComponentEmitter>(int set)
{
	ComponentEmitter_ind = set;
}


template<>
int typeResolver<ComponentPlayerStats>()
{
	return ComponentPlayerStats_ind;
}
template<>
void setTypeResolver<ComponentPlayerStats>(int set)
{
	ComponentPlayerStats_ind = set;
}


template<>
int typeResolver<ComponentCauldronStats>()
{
	return ComponentCauldronStats_ind;
}
template<>
void setTypeResolver<ComponentCauldronStats>(int set)
{
	ComponentCauldronStats_ind = set;
}


template<>
int typeResolver<ComponentCScript>()
{
	return ComponentCScript_ind;
}
template<>
void setTypeResolver<ComponentCScript>(int set)
{
	ComponentCScript_ind = set;
}


template<>
int typeResolver<ComponentVariables>()
{
	return ComponentVariables_ind;
}
template<>
void setTypeResolver<ComponentVariables>(int set)
{
	ComponentVariables_ind = set;
}


template<>
int typeResolver<ComponentNavigator>()
{
	return ComponentNavigator_ind;
}
template<>
void setTypeResolver<ComponentNavigator>(int set)
{
	ComponentNavigator_ind = set;
}


template<>
int typeResolver<ComponentWayPointMap>()
{
	return ComponentWayPointMap_ind;
}
template<>
void setTypeResolver<ComponentWayPointMap>(int set)
{
	ComponentWayPointMap_ind = set;
}


template<>
int typeResolver<ComponentWayPoint>()
{
	return ComponentWayPoint_ind;
}
template<>
void setTypeResolver<ComponentWayPoint>(int set)
{
	ComponentWayPoint_ind = set;
}
