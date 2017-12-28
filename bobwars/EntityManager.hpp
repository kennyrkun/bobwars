#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "BaseEntity.hpp"
#include <vector>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	std::vector<BaseEntity*> entities;
	std::vector<BaseEntity*> selectedEnts;

	void newEnt();
	void deleteEnt(BaseEntity *ent);

	int selectEnt(BaseEntity *ent);
	void deselectEnt(BaseEntity *ent);
	void deselectAllEnts();
	// add selected bool to entity, check if it's selected without looping.

	unsigned long long int maxEnts = std::numeric_limits<long long int>::max();

private:
	// nothing
};

#endif // ENTITY_MANAGER_HPP
