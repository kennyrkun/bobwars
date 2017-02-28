#ifndef ENGINE_MAIN_HPP
#define ENGINE_MAIN_HPP

#include <SFML\Graphics.hpp>

using CVar = bool;

namespace engine
{
	extern const std::string version;
	extern CVar cl_debug;

	namespace text
	{
		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, const sf::Color &c);
		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize);
		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);
	}

	std::string getTime();
}

#include "logger.hpp"

#endif
