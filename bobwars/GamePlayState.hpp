#ifndef GAME_HPP
#define GAME_HPP

#include "AppEngine.hpp"
#include "AppState.hpp"

#include "Camera.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>	

class EntityManager;
class Interface;

struct GameWorld
{
	void loadMapFile(std::string file);

	sf::Texture mapTexture;
	sf::RectangleShape map;

	size_t mapSizeX;
	size_t mapSizeY;
};

struct Resources
{
	int memes = 100;
};

class GamePlayState : public AppState
{
public:
	void Init(AppEngine* app_);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

private:
	AppEngine* app;

	sf::Time timePerFrame; // 60 frames per second
	sf::Time timeSinceLastUpdate;

	EntityManager *entMan;
	Interface *ui;

	sf::RectangleShape world;
	sf::Texture		   *worldTexture;

	sf::Font Arial;
	sf::Text debugFrameCounter;
	sf::Text debugText;

	// TODO: rename this
	Camera* mainView2;
	int baseViewSpeed = 500;

	sf::Clock deltaClock;  // tracks how much time has past since the last frame
	sf::Clock framesClock; // for maxfps

	void deleteButton();

	void showObjectCoords(sf::Sprite &object);

	void saveGame();
};

#endif /* GAME_HPP */