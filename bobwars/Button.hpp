#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <ENGINE\engine_main.hpp>

class Button
{
public:
	sf::RectangleShape m_shape;

	Button(const sf::Vector2f &window_size, const sf::Vector2f &size, const std::string string);
	~Button();

	void setPosition(const sf::Vector2f &pos);
	void setButtonString(const std::string string);
	void setButtonColor(const sf::Color &color);
	void setStringColor(const sf::Color &color);
	void draw(sf::RenderWindow &window);

private:
	sf::Text m_string;
	sf::Font m_font;
};

#endif
