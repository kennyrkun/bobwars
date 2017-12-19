#ifndef GAMEO_BJECT_MANAGER_HPP
#define GAME_OBJECT_MANAGER_HPP

#include "BaseEntity.hpp"
#include <vector>

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	std::vector<BaseEntity*> entities;
	std::vector<BaseEntity*> selectedEnts;

	void createNewObject();
	void deleteObject(BaseEntity *ent);

	int selectObject(BaseEntity *ent);
	void deselectObject(BaseEntity *ent);
	void deselectAllObjects();
	// add selected bool to entity, check if it's selected without looping.

private:
	// nothing
};

#endif // GAME_OBJECT_MANAGER_HPP
