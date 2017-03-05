#ifndef ENGINE_MAIN_HPP
#define ENGINE_MAIN_HPP

#include <SFML\Graphics.hpp>

using CVar = bool;

namespace engine
{
	extern const std::string version;
	extern CVar cl_debug;

	///Functions related to drawing text.
	namespace text
	{
		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, const sf::Color &c);
		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize);
		void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);
	}

	namespace graphics
	{
		void outline(sf::RenderWindow &window, sf::Shape &object, float thickness, sf::Color color);
		void outline(sf::RenderWindow &window, sf::Sprite &object, float thickness, sf::Color color);
	}

	std::string getTime();
}

#include "logger.hpp"

#endif // ENGINE_MAIN_HPP
