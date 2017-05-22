#ifndef GAME_HPP
#define GAME_HPP

class Button;
class ObjectManager;
class Interface;

#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>
#include <string>

class Game
{
public:
	Game(bool fullscreen, bool vsync);
	~Game();

	std::string gameVersion = "0.5.0";
	sf::RenderWindow *gameWindow;
	sf::Time timePerFrame; // 60 frames per second
	sf::Time timeSinceLastUpdate;

	void Main();
	void Update();
	void Render();

private:
	ObjectManager *obMan;
	Interface *ui;
	sf::RectangleShape world;
	sf::Font Arial;
	sf::Text frameCounter;
	sf::Text text;
	sf::View main_view;
	sf::View anchor;
	int view_speed = 500;
	int player_speed = 250;

	void showObjectCoords(sf::RenderWindow &window, sf::Sprite &object);
};

#endif /* GAME_HPP */