#include "EntityManager.hpp"

#include "BaseEntity.hpp"

#include "Util/Util.hpp"
#include "Util/Logger.hpp"

EntityManager::EntityManager()
{
	logger::DEBUG("ObjectManager constructed.", true);

	maxEntsPerTeam = maxEnts / util::maxPlayers;
}

EntityManager::~EntityManager()
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		delete entities[i];
		entities[i] = nullptr;
	}

	selectedEnts.clear();
	entities.clear();

	logger::DEBUG("ObjectManager deconstructed.", true);
}

BaseEntity* EntityManager::addEnt(BaseEntity* ent)
{
	entities.push_back(ent);

	if (ent->entityID != entities.size())
		logger::WARNING("Entity ID mismatch. " + std::to_string(ent->entityID) + " should be " + std::to_string(entities.size()));

	if (ent->isSelected)
		selectedEnts.push_back(ent);

	return ent;
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

	ent = nullptr;

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

bool EntityManager::selectEnt(BaseEntity *ent)
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
		return false;

	ent->isSelected = true;
	selectedEnts.push_back(ent);
	return true;
}

void EntityManager::deselectEnt(BaseEntity *ent)
{
	// FIXME: this function doesn't work?

	ent->isSelected = false;
	selectedEnts.erase(std::remove(selectedEnts.begin(), selectedEnts.end(), ent), selectedEnts.end());

	logger::DEBUG("deselected entity" + std::to_string(ent->entityID));
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

std::vector<BaseEntity*> EntityManager::getNearbyEntities(BaseEntity* entity, float radius)
{
	std::vector<BaseEntity*> list;
	list.reserve(entities.size());

	//sf::FloatRect area = {entity.getPosition().x, entity.getPosition().y, x * 2, y * 2};
	sf::RectangleShape area;
	area.setSize(sf::Vector2f(radius * 2, radius * 2));
	area.setOrigin(sf::Vector2f(area.getSize().x / 2, area.getSize().y / 2));
	area.setPosition(entity->getPosition());

	for (BaseEntity* e : entities)
	{
		if (e == entity)
			continue;

		if (area.getGlobalBounds().contains(e->getPosition()))
			list.push_back(e);
	}

	list.shrink_to_fit();
	return list;
}
