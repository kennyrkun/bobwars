#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML\Graphics.hpp>

namespace engine {
namespace text
{
	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, const sf::Color &c);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const sf::Vector2f& scale);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);
}
}

#endif
