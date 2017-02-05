#pragma once

#include <SFML\Graphics.hpp>

namespace engine
{
	void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const sf::Vector2f &scale);

	void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);

	std::string getTime();

	std::string getObjectCoords(sf::Sprite &object);
}

namespace logger
{
	void INFO(std::string output);

	void WARNING(std::string output);

	void ERROR(int type, std::string output);

//	void DEBUG(std::string output);
}
