#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>

namespace util {

namespace text
{
	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, const sf::Color &c);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const sf::Vector2f& scale);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);

	void draw(sf::RenderWindow &window, const std::string string, const sf::Vector2f &pos);
}

}

#endif
