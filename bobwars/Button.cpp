#include "Button.hpp"
#include <iostream>

Button::Button(const sf::Vector2f &window_size, const sf::Vector2f &size, const std::string string)
{
	if (m_font.loadFromFile("resource\\fonts\\arial.ttf"))
		m_text.setFont(m_font);

	m_text.setString(string);
	m_text.setFillColor(sf::Color::Black);

	m_shape.setFillColor(sf::Color::White);
	m_shape.setSize(sf::Vector2f(size.x, size.y));
	m_text.setCharacterSize(size.y);

	m_shape.setOrigin(m_shape.getLocalBounds().width / 2, m_shape.getLocalBounds().height / 2);
	m_text.setOrigin(m_text.getLocalBounds().width / 2, m_text.getLocalBounds().height);

	m_shape.setPosition(sf::Vector2f(window_size.x / 2, window_size.y / 2));
	m_text.setPosition(m_shape.getOrigin());

	//TODO: not have this set in stone
	m_shape.setScale(.2, .2);
	m_text.setScale(.2, .2);
}

Button::~Button()
{
	logger::DEBUG("deconstructing button class");
}

void Button::setPosition(const sf::Vector2f &pos)
{
	m_shape.setPosition(pos);
	m_text.setPosition(m_shape.getPosition());
}

void Button::draw(sf::RenderWindow &window)
{
	window.draw(m_shape);
	window.draw(m_text);
}
