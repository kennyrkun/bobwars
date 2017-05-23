#include "Text.hpp"

namespace engine {
namespace text
{
	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, const sf::Color &c)
	{
		text.setPosition(pos);
		text.setCharacterSize(characterSize);
		text.setFillColor(c);
		text.setString(string);

		window.draw(text);

		text.setFillColor(sf::Color::White);
	}

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize)
	{
		text.setPosition(pos);
		text.setCharacterSize(characterSize);
		text.setString(string);

		window.draw(text);
	}

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos)
	{
		text.setPosition(pos);
		text.setCharacterSize(34);
		text.setString(string);

		window.draw(text);
	}
}
}
