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
	newEnt->isSelected = true;

	entities.push_back(newEnt); // add it to the stack
	selectedEnts.push_back(newEnt); // select it

	logger::INFO("creating new entity (" + std::to_string(newEnt->id) + ")");
}

void ObjectManager::clearSelected()
{
	logger::INFO("selectedEntities: " + std::to_string(selectedEnts.size()));

	for (size_t i = 0; i < selectedEnts.size(); i++)
	{
		logger::INFO("deselecting entity" + std::to_string(selectedEnts[i]->id));

		//delesectEnt(selectedEnts[i]); // broken
		selectedEnts[i]->isSelected = false;
	}

	logger::INFO("selectedEntities: " + std::to_string(selectedEnts.size()));

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
			logger::INFO("Deleted entity " + std::to_string(ent->id));
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

void ObjectManager::deselectObject(BaseEntity *ent)
{
	// FIXME: this function doesn't work?

	ent->isSelected = false;
	selectedEnts.erase(std::remove(selectedEnts.begin(), selectedEnts.end(), ent), selectedEnts.end());

	logger::INFO("deselected entity" + std::to_string(ent->id));
}