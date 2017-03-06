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

	BaseEntity null;
	BaseEntity *selected = &null;

	void createObject();
	void deleteObject(BaseEntity *ent);
	void selectObject(BaseEntity *entity);

private:
	// nothing
};

#endif // GAME_OBJECT_MANAGER_HPP
