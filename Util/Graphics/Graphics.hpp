#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>

namespace util {
namespace graphics
{
	void outline(sf::RenderWindow &window, sf::Shape &object, float thickness, sf::Color color);
	void outline(sf::RenderWindow &window, sf::Sprite &object, float thickness, sf::Color color);

	void fade(sf::Shape &object, int opacity);
}
}

#endif