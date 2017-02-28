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
	void draw(sf::RenderWindow &window);

private:
	sf::Text m_text;
	sf::Font m_font;
};

#endif
