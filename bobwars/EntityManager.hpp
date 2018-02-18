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

	BaseEntity* newBaseEntity();
	BaseEntity* newBob();
	BaseEntity* newCommentSection();

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

	unsigned long long int physicalMaxEnts = std::numeric_limits<int>::max();
	unsigned long long int maxEnts = 8000;
	unsigned long long int maxEntsPerTeam;
};

#endif // ENTITY_MANAGER_HPP
