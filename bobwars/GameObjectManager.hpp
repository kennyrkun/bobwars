#ifndef GAMEO_BJECT_MANAGER_HPP
#define GAME_OBJECT_MANAGER_HPP

#include "BaseEntity.hpp"
#include <vector>

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

	std::vector<BaseEntity*> entites;

	void createObject()
	{
		BaseEntity *newEnt = new BaseEntity;
		entites.push_back(newEnt);
	}

	void deleteObject()
	{

	}

private:

};

#endif
