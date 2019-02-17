#include "EntityManager.hpp"

#include "Util\Util.hpp"
#include "Util\Logger.hpp"

#include "BaseEntity.hpp"

#include "Bob.hpp"
#include "CommentSection.hpp"

EntityManager::EntityManager()
{
	logger::DEBUG("ObjectManager constructed.", true);

	maxEntsPerTeam = maxEnts / util::maxPlayers;
}

EntityManager::~EntityManager()
{
	for (size_t i = 0; i < entities.size(); i++)
		delete entities[i];

//	is this needed?
//	in theory, there can be no more selected entities than entities,
//	so deleting all entities will also delete all selected entities
//	thus rendering this code uselss and crash-creaty
//	for (size_t i = 0; i < selectedEnts.size(); i++)
//		delete selectedEnts[i];

	selectedEnts.clear();
	entities.clear();

	logger::DEBUG("ObjectManager deconstructed.", true);
}

Bob* EntityManager::newBob()
{
	//TODO: optimise entity creation

	Bob *newBobEntity = new Bob();
	newBobEntity->entityID = entities.size() + 1;

	entities.push_back(newBobEntity); // add it to the stack

	logger::DEBUG("creating new bob entity (" + std::to_string(newBobEntity->entityID) + ")");
	return newBobEntity;
}

CommentSection* EntityManager::newCommentSection()
{
	//TODO: optimise entity creation
	//TODO: comment sections might not want to be selected on creation

	CommentSection *newBobEntity = new CommentSection();
	newBobEntity->entityID = entities.size() + 1;

	entities.push_back(newBobEntity);
				
	logger::DEBUG("created comment section entity (" + std::to_string(newBobEntity->entityID) + ")");
	return newBobEntity;
}

void EntityManager::deselectAllEnts()
{
	//TODO: optimise deselection methods

	for (size_t i = 0; i < selectedEnts.size(); i++)
	{
		logger::DEBUG("deselecting entity" + std::to_string(selectedEnts[i]->entityID));

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
//			logger::INFO("Deleted entity " + std::to_string(ent->entityID));
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

	logger::DEBUG("deselected entity" + std::to_string(ent->entityID));
}