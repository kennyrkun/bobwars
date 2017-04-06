#ifndef GAMEO_BJECT_MANAGER_HPP
#define GAME_OBJECT_MANAGER_HPP

#include "BaseEntity.hpp"
//#include "ResourceManager.hpp"
#include <vector>

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	std::vector<BaseEntity*> entities;
	int num_entities = 0; // number of entites in the stack

	BaseEntity *selected;

	void createObject();
	void deleteObject(BaseEntity *ent);
	void selectObject(BaseEntity *ent);  //inline

private:
	// nothing
};

#endif // GAME_OBJECT_MANAGER_HPP
