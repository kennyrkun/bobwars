#ifndef GAME_HPP
#define GAME_HPP

#include "AppEngine.hpp"
#include "AppState.hpp"

#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>
#include <string>

class Button;
class ObjectManager;
class Interface;

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

	static GamePlayState* Instance()
	{
		return &GamePlayState_dontfuckwithme;
	}

	std::string gameVersion = "0.8.2";
//	sf::RenderWindow *app->window;
	sf::Time timePerFrame; // 60 frames per second
	sf::Time timeSinceLastUpdate;

private:
	static GamePlayState GamePlayState_dontfuckwithme;
	AppEngine* app;

	ObjectManager *obMan;
	Interface *ui;
	sf::RectangleShape world;
	sf::Texture		   worldTexture;

	sf::Font Arial;
	sf::Text debugFrameCounter;
	sf::Text debugText;

	sf::View *mainView;
	sf::View *viewAnchor;
	int baseViewSpeed = 500;

	sf::Clock deltaClock;  // tracks how much time has past since the last frame
	sf::Clock framesClock; // for fps

	void showObjectCoords(sf::Sprite &object);
};

#endif /* GAME_HPP */