#pragma once

#include <SFML\Graphics.hpp>

using CVar = bool;

namespace engine
{
	extern std::string build_number;
	extern CVar cl_debug;

	namespace text
	{
		void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, sf::Color c);
		void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize);
		void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);
	}

	std::string getTime();
}

#include "logger.hpp"
