#pragma once

#include <SFML\Graphics.hpp>

namespace engine
{
	extern std::string build;

	void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const sf::Vector2f &scale);

	void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);

	std::string getTime();

	std::string getObjectCoords(sf::Sprite &object);
}

namespace logger
{
	extern std::string last_message;

	void INFO(std::string output);

	void WARNING(std::string output);

	void ERROR(int type, std::string output);

//	void DEBUG(std::string output);
}
