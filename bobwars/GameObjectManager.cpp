#include "GameObjectManager.hpp"

GameObjectManager::GameObjectManager()
{
	logger::DEBUG("ObjectManager constructed.");

	BaseEntity* null_entity = new BaseEntity;
	entites.push_back(null_entity); // because we need an null.
}

GameObjectManager::~GameObjectManager()
{
	logger::DEBUG("ObjectManager deconstructed.");

	delete &entites;
}

