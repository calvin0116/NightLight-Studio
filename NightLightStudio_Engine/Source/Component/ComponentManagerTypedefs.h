#pragma once


typedef NS_COMPONENT::ComponentManager::ComponentSetManager::EntityHandle Entity;

typedef NS_COMPONENT::ComponentManager::ComponentSetManager::Iterator ComponentIterator;

typedef NS_COMPONENT::ComponentManager::ComponentSetManager::Iterator::IteratorState ComponentIteratorState;

static NS_COMPONENT::ComponentManager* SYS_COMPONENT = NS_COMPONENT::ComponentManager::GetInstance();

//extern ComponentManager G_COMPMGR;

// Entity component manager
static NS_COMPONENT::ComponentManager::ComponentSetManager* G_ECMANAGER = SYS_COMPONENT->getComponentSetMgr(NS_COMPONENT::COMPONENT_MAIN);

// Entity component manager UI
static NS_COMPONENT::ComponentManager::ComponentSetManager* G_ECMANAGER_UI = SYS_COMPONENT->getComponentSetMgr(NS_COMPONENT::COMPONENT_UI);

// Entity component manager Prefabs
static NS_COMPONENT::ComponentManager::ComponentSetManager* G_ECMANAGER_PREFABS = SYS_COMPONENT->getComponentSetMgr(NS_COMPONENT::COMPONENT_PREFABS);

// for some reason gfx becomes black screen with this
//static ComponentManager::ComponentSetManager* G_UICOMPSET = SYS_COMPONENT->getComponentSetMgr(COMPONENT_MAIN);

