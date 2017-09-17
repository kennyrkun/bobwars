#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML\Graphics.hpp>
#include <ENGINE\Logger.hpp>

namespace engine
{
	extern const std::string version;

	namespace logic
	{
		bool mouseIsOver(sf::Shape &object, sf::RenderWindow &window, sf::View &view);

		bool mouseIsOver(sf::Sprite &object, sf::RenderWindow &window, sf::View &view);
	}

	void screenshot(sf::RenderWindow &window);

	std::string getTimestamp();
}

#include "logger.hpp"

#endif // ENGINE_MAIN_HPP
