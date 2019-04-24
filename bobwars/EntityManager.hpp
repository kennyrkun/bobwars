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

	CommentSection* newCommentSection();

	void addEnt(BaseEntity *ent);
	void deleteEnt(BaseEntity *ent);

	bool selectEnt(BaseEntity *ent);
	void deselectEnt(BaseEntity *ent);
	void deselectAllEnts();
	// add selected bool to entity, check if it's selected without looping.

	unsigned long long int physicalMaxEnts = std::numeric_limits<unsigned long long int>::max();
	unsigned long long int maxEnts = 99999999;
	unsigned long long int maxEntsPerTeam;
};

#endif // ENTITY_MANAGER_HPP
