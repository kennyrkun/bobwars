#include "ObjectManager.hpp"

#include <algorithm> // std::remove

ObjectManager::ObjectManager()
{
	logger::SILENT("DEBUG", "ObjectManager constructed.");
}

ObjectManager::~ObjectManager()
{
	for (size_t i = 0; i < entities.size(); i++)
		delete entities[i];

	logger::SILENT("DEBUG", "ObjectManager deconstructed.");
}

void ObjectManager::createObject()
{
	BaseEntity *newEnt = new BaseEntity();
	newEnt->id = entities.size() + 1;

	entities.push_back(newEnt); // add it to the stack
	selectedEnts.push_back(newEnt); // select it

	logger::SILENT("DEBUG", "creating new entity");
}

void ObjectManager::clearSelected()
{
	selectedEnts.clear();
}

void ObjectManager::deleteObject(BaseEntity *ent)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (ent == entities[i])
		{
			delete entities[i];
			entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());
			logger::SILENT("DEBUG", "Deleted entity " + std::to_string(ent->id));
			return;
		}
	}
}

int ObjectManager::selectObject(BaseEntity *ent)
{
	for (size_t i = 0; i < selectedEnts.size(); i++)
	{
		if (ent == selectedEnts[i])
		{
			logger::WARNING("This entity was already selected!");
			return 0;
		}
	}

	selectedEnts.push_back(ent);
	return 1;
}
