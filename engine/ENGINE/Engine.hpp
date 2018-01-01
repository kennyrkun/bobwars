#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <ENGINE/Logger.hpp>

#include <SFML/Graphics.hpp>

namespace engine
{
	extern const std::string version;

	extern const int maxPlayers;

	namespace logic
	{
		bool mouseIsOver(sf::Shape &object, sf::RenderWindow &window, sf::View &view);

		bool mouseIsOver(sf::Shape &object, sf::RenderWindow &window);

		bool mouseIsOver(sf::Sprite &object, sf::RenderWindow &window, sf::View &view);

		bool mouseIsOver(sf::Sprite &object, sf::RenderWindow &window);
	}

	void screenshot(sf::RenderWindow &window);

	std::string getTimestamp();
}

#include "logger.hpp"

#endif // ENGINE_MAIN_HPP
