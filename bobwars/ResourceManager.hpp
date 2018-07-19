#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML/Graphics.hpp>

#include <map>
#include <string>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void print();

	// TODO: this should probably be a bool
	// to return whether or not we loaded the texture
	sf::Texture* loadTexture(std::string resourceName, std::string fileLocation);
	bool textureLoaded(std::string filename);
	sf::Texture* getTexture(std::string filename);
	void freeTexture(std::string filename);
	void freeAllTextures();

	// TODO: audio

	// TODO: fonts

private:
	std::map<std::string, sf::Texture*> loaded_textures;
};

#endif
