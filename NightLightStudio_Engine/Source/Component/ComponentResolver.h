#pragma once


//
//#include "..//Component/Components.h"


class ComponentTransform;
struct ComponentCollider;
struct ComponentRigidBody;
class ComponentGraphics;
class ComponentAudio;
class ComponentLoadAudio;
class ComponentScript;
class ComponentLight;
class ComponentCanvas;
class ComponentAnimation;
class ComponentEmitter;
struct ComponentPlayerStats;
struct ComponentCauldronStats;
class ComponentCScript;
struct ComponentVariables;
class ComponentNavigator;
class ComponentWayPointMap;
class ComponentWayPoint;


template<typename T>
int typeResolver()
{
	return -1;
}
template<typename T>
void setTypeResolver(int)
{
}

static int ComponentTransform_ind = -1;
template<>
int typeResolver<ComponentTransform>();
template<>
void setTypeResolver<ComponentTransform>(int set);


static int ComponentCollider_ind = -1;
template<>
int typeResolver<ComponentCollider>();
template<>
void setTypeResolver<ComponentCollider>(int set);

static int ComponentRigidBody_ind = -1;
template<>
int typeResolver<ComponentRigidBody>();
template<>
void setTypeResolver<ComponentRigidBody>(int set);

static int ComponentGraphics_ind = -1;
template<>
int typeResolver<ComponentGraphics>();
template<>
void setTypeResolver<ComponentGraphics>(int set);

static int ComponentAudio_ind = -1;
template<>
int typeResolver<ComponentAudio>();
template<>
void setTypeResolver<ComponentAudio>(int set);

static int ComponentLoadAudio_ind = -1;
template<>
int typeResolver<ComponentLoadAudio>();
template<>
void setTypeResolver<ComponentLoadAudio>(int set);

static int ComponentScript_ind = -1;
template<>
int typeResolver<ComponentScript>();
template<>
void setTypeResolver<ComponentScript>(int set);

static int ComponentLight_ind = -1;
template<>
int typeResolver<ComponentLight>();
template<>
void setTypeResolver<ComponentLight>(int set);

static int ComponentCanvas_ind = -1;
template<>
int typeResolver<ComponentCanvas>();
template<>
void setTypeResolver<ComponentCanvas>(int set);

static int ComponentAnimation_ind = -1;
template<>
int typeResolver<ComponentAnimation>();
template<>
void setTypeResolver<ComponentAnimation>(int set);

static int ComponentEmitter_ind = -1;
template<>
int typeResolver<ComponentEmitter>();
template<>
void setTypeResolver<ComponentEmitter>(int set);

static int ComponentPlayerStats_ind = -1;
template<>
int typeResolver<ComponentPlayerStats>();
template<>
void setTypeResolver<ComponentPlayerStats>(int set);

static int ComponentCauldronStats_ind = -1;
template<>
int typeResolver<ComponentCauldronStats>();
template<>
void setTypeResolver<ComponentCauldronStats>(int set);

static int ComponentCScript_ind = -1;
template<>
int typeResolver<ComponentCScript>();
template<>
void setTypeResolver<ComponentCScript>(int set);

static int ComponentVariables_ind = -1;
template<>
int typeResolver<ComponentVariables>();
template<>
void setTypeResolver<ComponentVariables>(int set);

static int ComponentNavigator_ind = -1;
template<>
int typeResolver<ComponentNavigator>();
template<>
void setTypeResolver<ComponentNavigator>(int set);

static int ComponentWayPointMap_ind = -1;
template<>
int typeResolver<ComponentWayPointMap>();
template<>
void setTypeResolver<ComponentWayPointMap>(int set);

static int ComponentWayPoint_ind = -1;
template<>
int typeResolver<ComponentWayPoint>();
template<>
void setTypeResolver<ComponentWayPoint>(int set);
