#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "BaseEntity.hpp"
#include "Bob.hpp"

#include <vector>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	std::vector<BaseEntity*> entities;
	std::vector<BaseEntity*> selectedEnts;

	void newBaseEntity();
	void newBob();

	// newBob
	// newBob(position);
	// newGPlus
	// newGPlus(position);

	void addEnt(BaseEntity *ent);
	void deleteEnt(BaseEntity *ent);

	int selectEnt(BaseEntity *ent);
	void deselectEnt(BaseEntity *ent);
	void deselectAllEnts();
	// add selected bool to entity, check if it's selected without looping.

	unsigned long long int physicalMaxEnts = std::numeric_limits<long long int>::max();
	unsigned long long int maxEnts = 500000;
	unsigned long long int maxEntsPerTeam;

private:
	// nothing
};

#endif // ENTITY_MANAGER_HPP
