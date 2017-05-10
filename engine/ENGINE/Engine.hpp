#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML\Graphics.hpp>
#include <ENGINE\Logger.hpp>

using CVar_B = bool;
using CVar_I = int;

namespace engine
{
	extern const std::string version;
	extern CVar_B cl_debug;

	namespace graphics
	{
		void outline(sf::RenderWindow &window, sf::Shape &object, float thickness, sf::Color color);
		void outline(sf::RenderWindow &window, sf::Sprite &object, float thickness, sf::Color color);

		void fade(sf::Shape &object, int opacity);
	}

	namespace logic
	{
		bool didClick(sf::Shape &object, sf::RenderWindow &window, sf::View &view);

		bool didClick(sf::Sprite &object, sf::RenderWindow &window, sf::View &view);
	}

	void screenshot(sf::RenderWindow &window);

	std::string getTimestamp();
}

#include "logger.hpp"

#endif // ENGINE_MAIN_HPP
