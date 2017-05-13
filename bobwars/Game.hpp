#ifndef GAME_HPP
#define GAME_HPP

#include "ObjectManager.hpp"
#include "Button.hpp"
#include "Interface.hpp"

#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>
#include <string>

class Game
{
public:
	Game(bool $fullscreen);
	~Game();

	std::string gameVersion = "0.4.6";
	sf::RenderWindow *gameWindow;

	void Main();
	void Update();
	void Render();

private:
	ObjectManager obMan;
	Interface *ui;
	sf::RectangleShape world;
	sf::Font Arial;
	sf::Text frameCounter;
	sf::Text text;
	sf::View main_view;
	sf::View anchor;
	int view_speed = 500;
	int player_speed = 250;
	bool cl_debug;

	void showObjectCoords(sf::RenderWindow &window, sf::Sprite &object);
};

#endif /* GAME_HPP */