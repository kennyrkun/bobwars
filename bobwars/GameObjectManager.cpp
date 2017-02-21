#include "GameObjectManager.hpp"

GameObjectManager::GameObjectManager()
{
	logger::INFO("object manager instance created.");

	BaseEntity* null_entity = new BaseEntity;
	entites.push_back(null_entity); // because we need an null.
}

GameObjectManager::~GameObjectManager()
{
	logger::INFO("object manager instance removed.");

	delete &entites;
}

