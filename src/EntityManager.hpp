#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "BaseEntity.hpp"
#include "Bob.hpp"
#include "CommentSection.hpp"

#include <vector>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	std::vector<BaseEntity*> entities;
	std::vector<BaseEntity*> selectedEnts;

	Bob* newBob();

	// TODO: put this definition in EntityManager.cpp so we can use logger
	template <typename T>
	inline T* create()
	{
		T* newEntity = new T(entities.size() + 1);

		entities.push_back(newEntity);
				
//		logger::DEBUG("created " + newEntity->type + " entity (" + std::to_string(newEntity->entityID) + ")");
		return newEntity;
	}

	inline int getNextID()
	{
		return totalEntities += 1;
	}

	void addEnt(BaseEntity *ent);
	void deleteEnt(BaseEntity *ent);

	bool selectEnt(BaseEntity *ent);
	void deselectEnt(BaseEntity *ent);
	void deselectAllEnts();
	// add selected bool to entity, check if it's selected without looping.

	// TODO: Filter: list of Types that we don't care about 
	std::vector<BaseEntity*> getNearbyEntities(BaseEntity* entity, float radius);

	unsigned long long int physicalMaxEnts = std::numeric_limits<unsigned long long int>::max();
	unsigned long long int maxEnts = 100000; // TODO: rename to entityLimit
	unsigned long long int maxEntsPerTeam;

	unsigned long long int totalEntities;
};

#endif // ENTITY_MANAGER_HPP
