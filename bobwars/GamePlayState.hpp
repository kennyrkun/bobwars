#ifndef GAME_HPP
#define GAME_HPP

#include "StateMachine.hpp"
#include "StateBase.hpp"

#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>
#include <string>

class Button;
class ObjectManager;
class Interface;

class GamePlayState : public AppState2
{
public:
	GamePlayState(AppEngine2* app_, bool fullscreen, bool vsync);
	~GamePlayState();

	void Pause();
	void Resume();

	void HandleEvents();
	void Update();
	void Draw();

	std::string gameVersion = "0.7.3";
//	sf::RenderWindow *app->window;
	sf::Time timePerFrame; // 60 frames per second
	sf::Time timeSinceLastUpdate;

private:
	AppEngine2* app;

	ObjectManager *obMan;
	Interface *ui;
	sf::RectangleShape world;
	sf::Texture		   worldTexture;
	sf::Font Arial;
	sf::Text frameCounter;
	sf::Text text;
	sf::View *mainView;
	sf::View *viewAnchor;
	int baseViewSpeed = 500;

	sf::Clock deltaClock;  // tracks how much time has past since the last frame
	sf::Clock framesClock; // for fps

	void showObjectCoords(sf::Sprite &object);
};

#endif /* GAME_HPP */