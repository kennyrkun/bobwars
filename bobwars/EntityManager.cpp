#include "EntityManager.hpp"

#include <ENGINE\Engine.hpp>
#include <ENGINE\Logger.hpp>

#include "BaseEntity.hpp"

#include "Bob.hpp"
#include "CommentSection.hpp"

EntityManager::EntityManager()
{
	logger::SILENT("DEBUG", "ObjectManager constructed.");

	maxEntsPerTeam = maxEnts / engine::maxPlayers;
}

EntityManager::~EntityManager()
{
	for (size_t i = 0; i < selectedEnts.size(); i++)
		delete selectedEnts[i];

	for (size_t i = 0; i < entities.size(); i++)
		delete entities[i];

	logger::SILENT("DEBUG", "ObjectManager deconstructed.");
}

BaseEntity* EntityManager::newBaseEntity()
{
	//TODO: optimise entity creation

	BaseEntity *newBaseEntity = new BaseEntity();
	newBaseEntity->id = entities.size() + 1;
	newBaseEntity->isSelected = true;

	entities.push_back(newBaseEntity); // add it to the stack
	selectedEnts.push_back(newBaseEntity); // select it

	return newBaseEntity;
//	logger::INFO("creating new entity (" + std::to_string(newEnt->id) + ")");
}

BaseEntity* EntityManager::newBob()
{
	//TODO: optimise entity creation

	Bob *newBobEntity = new Bob();
	newBobEntity->id = entities.size() + 1;
	newBobEntity->isSelected = true;

	entities.push_back(newBobEntity); // add it to the stack
	selectedEnts.push_back(newBobEntity); // select it

	return newBobEntity;
									   
//	logger::INFO("creating new entity (" + std::to_string(newEnt->id) + ")");
}

BaseEntity* EntityManager::newCommentSection()
{
	//TODO: optimise entity creation
	//TODO: comment sections might not want to be selected on creation

	CommentSection *newBobEntity = new CommentSection();
	newBobEntity->id = entities.size() + 1;
	newBobEntity->isSelected = true;

	entities.push_back(newBobEntity); // add it to the stack
	selectedEnts.push_back(newBobEntity); // select it
				
	return newBobEntity;
//	logger::INFO("creating new entity (" + std::to_string(newEnt->id) + ")");
}

void EntityManager::deselectAllEnts()
{
	//TODO: optimise deselection methods

	for (size_t i = 0; i < selectedEnts.size(); i++)
	{
		logger::INFO("deselecting entity" + std::to_string(selectedEnts[i]->id));

		//delesectEnt(selectedEnts[i]); // broken
		selectedEnts[i]->isSelected = false;
	}

	selectedEnts.clear();
}

void EntityManager::addEnt(BaseEntity* ent)
{
	entities.push_back(ent);

	if (ent->isSelected)
		selectedEnts.push_back(ent);
}

void EntityManager::deleteEnt(BaseEntity* ent)
{
	//TODO: optimise entity deletion;

	bool wasSelected = ent->isSelected;
	//HACK: there has to be a way to do this cleanly without creating a bool for it
	delete ent;

	if (wasSelected)
		selectedEnts.erase(std::remove(selectedEnts.begin(), selectedEnts.end(), ent), selectedEnts.end());

	entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());

//	for (size_t i = 0; i < entities.size(); i++)
//	{
//		if (ent == entities[i])
//		{
//			delete entities[i];
//			selectedEnts.erase(std::remove(selectedEnts.begin(), selectedEnts.end(), ent), selectedEnts.end());
//			entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());
//			logger::INFO("Deleted entity " + std::to_string(ent->id));
//			return;
//		}
//	}
}

int EntityManager::selectEnt(BaseEntity *ent)
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

void EntityManager::deselectEnt(BaseEntity *ent)
{
	// FIXME: this function doesn't work?

	ent->isSelected = false;
	selectedEnts.erase(std::remove(selectedEnts.begin(), selectedEnts.end(), ent), selectedEnts.end());

	logger::INFO("deselected entity" + std::to_string(ent->id));
}