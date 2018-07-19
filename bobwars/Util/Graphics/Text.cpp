#include "Text.hpp"

namespace util {
namespace text
{
	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize, const sf::Color &c)
	{
		int originalCharacterSize = text.getCharacterSize();
		sf::Color originalColor = text.getFillColor();

		text.setPosition(pos);
		text.setCharacterSize(characterSize);
		text.setFillColor(c);
		text.setString(string);

		window.draw(text);

		text.setFillColor(originalColor);
		text.setCharacterSize(originalCharacterSize);
	}

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const int characterSize)
	{
		int originalCharacterSize = text.getCharacterSize();

		text.setPosition(pos);
		text.setCharacterSize(characterSize);
		text.setString(string);

		window.draw(text);

		text.setCharacterSize(originalCharacterSize);
	}

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos, const sf::Vector2f& scale)
	{
		sf::Vector2f originalScale = text.getScale();

		text.setPosition(pos);
		text.setString(string);

		window.draw(text);

		text.scale(originalScale);
	}

	void draw(sf::RenderWindow &window, sf::Text &text, const std::string string, const sf::Vector2f &pos)
	{
		text.setPosition(pos);
		text.setString(string);

		window.draw(text);
	}

	void draw(sf::RenderWindow &window, const std::string string, const sf::Vector2f &pos)
	{
		sf::Text text;
		sf::Font font;
		font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
		text.setFont(font);
		text.setCharacterSize(12);
		text.setPosition(pos);
		text.setString(string);

		window.draw(text);
	}
}
}
