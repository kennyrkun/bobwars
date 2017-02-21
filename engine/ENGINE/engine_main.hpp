#ifndef ENGINE_MAIN_HPP
#define ENGINE_MAIN_HPP

#include <SFML\Graphics.hpp>

using CVar = bool;

namespace engine
{
	extern const std::string build_number;
	extern CVar cl_debug;

	namespace text
	{
		void drawText(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, sf::Color c);
		void drawText(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize);
		void drawText(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);
	}

	std::string getTime();
}

#include "logger.hpp"

#endif
