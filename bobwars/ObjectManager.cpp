#include "ObjectManager.hpp"

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

void ObjectManager::createNewEntity()
{
	BaseEntity *newEnt = new BaseEntity();
	newEnt->id = entities.size() + 1;

	entities.push_back(newEnt); // add it to the stack
	selectedEnts.push_back(newEnt); // select it

	logger::SILENT("DEBUG", "creating new entity (" + std::to_string(newEnt->id) + ")");
}

void ObjectManager::clearSelected()
{
	for (size_t i = 0; i < selectedEnts.size(); i++)
		selectedEnts[i]->isSelected = false;

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
	//TODO: should this be here, or in the event loop?

//	for (size_t i = 0; i < selectedEnts.size(); i++)
//	{
//		if (ent == selectedEnts[i])
//		{
//			logger::INFO("This entity was already selected!");
//			return 0;
//		}
//	}

	if (ent->isSelected)
		return 0;

	ent->isSelected = true;
	selectedEnts.push_back(ent);
	return 1;
}
