#include "ObjectManager.hpp"

ObjectManager::ObjectManager()
{
	null.m_id = 1;
	entities.push_back(&null); // because we need a null.

	null = *entities[0];

	logger::DEBUG("ObjectManager constructed.");
}

ObjectManager::~ObjectManager()
{
	// this breaks things. don't do it.
	//for (size_t i = 0; i < entities.size(); i++)
		//delete &entities[i];

	logger::DEBUG("ObjectManager deconstructed.");
}

void ObjectManager::createObject()
{
	logger::DEBUG("created new entity");

	BaseEntity *newEnt = new BaseEntity();
	newEnt->m_id += 1; // how the fuck does this work.

	entities.push_back(newEnt); // add it to the stack
	selected = entities.back(); // select it
}

void ObjectManager::deleteObject(BaseEntity *ent)
{
	logger::DEBUG("deleted entity");

	delete &ent;
}

inline void ObjectManager::selectObject(BaseEntity *ent)
{
	selected = ent;
}
