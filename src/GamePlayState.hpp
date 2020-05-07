#ifndef GAME_PLAY_STATE_HPP
#define GAME_PLAY_STATE_HPP

#include "AppEngine.hpp"
#include "AppState.hpp"
#include "Camera.hpp"

#include <SFML/Graphics.hpp>

#include <string>

class GameSimulation;
class EntityManager;
class Interface;
class BaseEntity;
enum class EntityType;

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
	Interface *ui;

	sf::Text debugFrameCounter;
	sf::Text debugText;

	sf::RectangleShape rectSelect;
	bool rectSelecting = false;

	// TODO: rename this
	Camera* mainView2;
	int baseViewSpeed = 1;

	sf::Clock framesClock; // for maxfps

	GameSimulation* simulation;

	void updateGameCamera();

	void showObjectCoords(sf::Sprite &object);

	void saveGame();
	void loadGame();
};

#endif // !GAME_PLAY_STATE_HPP
