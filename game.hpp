#pragma once

#include <SFML\Graphics.hpp>

sf::View main_view(sf::Vector2f(400, 300), sf::Vector2f(400, 300));

sf::Sprite player;
sf::RectangleShape world(sf::Vector2f(800, 600));
sf::Texture world_tex;
sf::Texture player_tex;

sf::Event event;

class Game
{
public:
	bool paused;
private:
	bool debug;
};

Game game;

namespace GUI
{
	sf::RectangleShape bottom(sf::Vector2f(800, 50));
	sf::RectangleShape top(sf::Vector2f(800, 15));

	void init();
}
