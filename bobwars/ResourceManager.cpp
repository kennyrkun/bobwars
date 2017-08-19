#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

#include "ResourceManager.hpp"
#include <ENGINE\Logger.hpp>

ResourceManager::ResourceManager()
{
	logger::INFO("ResourceManager created.");
}

ResourceManager::~ResourceManager()
{
	freeAllTextures();

	logger::INFO("ResourceManager deconstructed.");
}

void ResourceManager::loadTexture(std::string filename)
{
	if (!textureLoaded(filename))
	{
		// If the texture is not in the map, load it
		sf::Texture* new_tex = new sf::Texture;
		new_tex->loadFromFile(filename);
		loaded_textures[filename] = new_tex;

		logger::INFO("Loaded texture \"" + filename + "\"");
	}
}

void ResourceManager::freeTexture(std::string filename)
{
	delete loaded_textures[filename];
	loaded_textures[filename] = nullptr;

	logger::INFO("Freed texture \"" + filename + "\"");
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

	logger::INFO("Freed all textures.");
}

bool ResourceManager::textureLoaded(std::string filename)
{
	// how does this work

	if (loaded_textures.find(filename) == loaded_textures.end())
		return false;
	else
		return true;
}

sf::Texture* ResourceManager::getTexture(std::string filename)
{
//	loadTexture(filename);
	return loaded_textures[filename];
}
