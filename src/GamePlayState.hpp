#ifndef GAME_PLAY_STATE_HPP
#define GAME_PLAY_STATE_HPP

#include "AppEngine.hpp"
#include "AppState.hpp"

#include "Camera.hpp"

#include <SFML/Graphics.hpp>

#include <string>

class EntityManager;
class Interface;
class BaseEntity;
enum class EntityType;

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

	sf::Clock googleTimer;
	sf::Clock resourceTimer;

	EntityManager *entMan;
	Interface *ui;

	sf::RectangleShape world;
	sf::Texture		   *worldTexture;

	sf::Text debugFrameCounter;
	sf::Text debugText;

	sf::RectangleShape rectSelect;
	bool rectSelecting = false;

	// TODO: rename this
	Camera* mainView2;
	int baseViewSpeed = 1;

	sf::Clock framesClock; // for maxfps
	sf::Clock gameClock; // total time of the game

	void updateGameCamera();

	void createEntity(EntityType type, const sf::Vector2f& position);
	void deleteEntities(const std::vector<BaseEntity*>& entities);

	void deleteButton();

	void showObjectCoords(sf::Sprite &object);

	void saveGame();
};

#endif // !GAME_PLAY_STATE_HPP
