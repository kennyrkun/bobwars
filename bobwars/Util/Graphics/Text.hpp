#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>

namespace util {

namespace text
{
	// TODO: we might use a different method to render text.
	// perhaps create a string manager type class, and use only one piece of text to render everything with line breaks
	// this could yeild a generous performance improvement.

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, const sf::Color &c);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const sf::Vector2f& scale);

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos);
}

}

#endif
