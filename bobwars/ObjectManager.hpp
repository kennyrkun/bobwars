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

	void createObject();
	void clearSelected();
	void deleteObject(BaseEntity *ent);
	int selectObject(BaseEntity *ent);

private:
	// nothing
};

#endif // GAME_OBJECT_MANAGER_HPP
