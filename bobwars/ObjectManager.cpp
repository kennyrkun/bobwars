#include "ObjectManager.hpp"

#include <algorithm> // std::remove

ObjectManager::ObjectManager()
{
	BaseEntity *null = new BaseEntity;

	num_entities += 1;
	null->m_id = num_entities;
	null->m_sprite.setColor(sf::Color::Red);

	entities.push_back(null);
	selected = entities[0];

	logger::SILENT("DEBUG", "ObjectManager constructed.");
}

ObjectManager::~ObjectManager()
{
	// this breaks things. don't do it.
	//for (size_t i = 0; i < entities.size(); i++)
		//delete &entities[i];

	logger::SILENT("DEBUG", "ObjectManager deconstructed.");
}

void ObjectManager::createObject()
{
	BaseEntity *newEnt = new BaseEntity();
	num_entities += 1;
	newEnt->m_id = num_entities;

	entities.push_back(newEnt); // add it to the stack
	selectObject(entities.back());

	logger::SILENT("DEBUG", "creating new entity");
}

void ObjectManager::deleteObject(BaseEntity *ent)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		if (ent == entities[i])
		{
			delete entities[i];

			entities.erase(std::remove(entities.begin(), entities.end(), ent), entities.end());

			num_entities -= 1;
			selected = entities.front();

			logger::SILENT("DEBUG", "deleted entity");

			return;
		}
	}
}

inline void ObjectManager::selectObject(BaseEntity *ent)
{
	selected = ent;
}
