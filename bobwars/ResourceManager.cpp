#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

#include "ResourceManager.hpp"
#include "Util/Logger.hpp"

ResourceManager::ResourceManager()
{
	logger::INFO("[RESMAN] ResourceManager created.");
}

ResourceManager::~ResourceManager()
{
	freeAllTextures();

	logger::INFO("[RESMAN] ResourceManager deconstructed.");
}

void ResourceManager::print()
{
	logger::INFO("NOT BROKEN!");
}

sf::Texture* ResourceManager::loadTexture(std::string resourceName, std::string fileLocation)
{
	if (!textureLoaded(resourceName))
	{
		sf::Texture* new_tex = new sf::Texture;
		new_tex->loadFromFile(fileLocation);
		loaded_textures[resourceName] = new_tex;

		logger::INFO("[RESMAN] Loaded \"" + resourceName + "\"");

		return loaded_textures[resourceName];
	}
	else
	{
		logger::ERROR("[RESMAN] " + resourceName + " is already loaded.");
	}
}

void ResourceManager::freeTexture(std::string filename)
{
	delete loaded_textures[filename];
	loaded_textures[filename] = nullptr;

	std::map<std::string, sf::Texture*>::iterator it;

	it = loaded_textures.find(filename);
	loaded_textures.erase(it);

	logger::INFO("[RESMAN] Freed texture \"" + filename + "\"");
}

void ResourceManager::freeAllTextures()
{
	// Iterate through all the loaded textures, free them, and delete the pointers
	std::map<std::string, sf::Texture*>::iterator it;

	for (it = loaded_textures.begin(); it != loaded_textures.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}

	loaded_textures.clear();

	logger::INFO("[RESMAN] Freed all textures.");
}

bool ResourceManager::textureLoaded(std::string filename)
{
	// how does this even what

	if (loaded_textures.find(filename) == loaded_textures.end())
		return false;
	else
		return true;
}

sf::Texture* ResourceManager::getTexture(std::string filename)
{
	//	loadTexture(filename); // no
	return loaded_textures[filename];
}
