#include "ObjectManager.hpp"

ObjectManager::ObjectManager()
{
	BaseEntity* null = new BaseEntity;
	null->m_id = 1;

	entities.push_back(null); // because we need a null.

	logger::DEBUG("ObjectManager constructed.");
}

ObjectManager::~ObjectManager()
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		delete &entities[i];
	}

	logger::DEBUG("ObjectManager deconstructed.");
}

void ObjectManager::createObject()
{
	logger::DEBUG("created new entity");

	BaseEntity *newEnt = new BaseEntity();
	newEnt->m_id += 1;
//	newEnt->m_sprite.setTexture(player_tex); ADDED WHEN RESOURCE MANAGER IS IMPLEMENTED

	entities.push_back(newEnt); // add it to the stack
	selected = entities.back(); // select it
}

void ObjectManager::deleteObject(BaseEntity *ent)
{
	logger::DEBUG("deleted entity");

	delete &ent;
}

void ObjectManager::select(BaseEntity *entity)
{
	selected = entity;
}
