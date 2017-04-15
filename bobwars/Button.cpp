#include "Button.hpp"
#include <iostream>

Button::Button(const sf::Vector2f &window_size, const sf::Vector2f &size, const std::string string)
{
	if (m_font.loadFromFile("resource\\fonts\\arial.ttf"))
		m_string.setFont(m_font);

	m_string.setString(string);
	m_string.setFillColor(sf::Color::Black);

	m_shape.setFillColor(sf::Color::White);

	m_shape.setSize(sf::Vector2f(size.x, size.y));
	m_string.setCharacterSize( static_cast<int>(size.y) );

	m_shape.setOrigin(m_shape.getLocalBounds().width / 2, m_shape.getLocalBounds().height / 2);
	m_string.setOrigin(m_string.getLocalBounds().width / 2, m_string.getLocalBounds().height - (m_string.getLocalBounds().height / 6));

//	m_shape.setPosition(sf::Vector2f(window_size.x / 2, window_size.y / 2));
	m_string.setPosition(m_shape.getOrigin());

	//TODO: not have this set in stone
	m_shape.setScale(.2f, .2f);
	m_string.setScale(.2f, .2f);

	logger::DEBUG("Button class constructed.");
}

Button::~Button()
{
	logger::DEBUG("Deconstructing button class");
}

void Button::setPosition(const sf::Vector2f &pos)
{
	m_shape.setPosition(pos);
	m_string.setPosition(m_shape.getPosition());
}

void Button::setButtonString(const std::string string)
{
	m_string.setString(string);

	m_shape.setSize(sf::Vector2f(m_string.getLocalBounds().width + 60, m_string.getLocalBounds().height + 13));
	m_string.setPosition(sf::Vector2f(m_shape.getPosition().x, m_shape.getPosition().y + 1));
}

void Button::setButtonColor(const sf::Color &color)
{
	m_shape.setFillColor(color);
}

void Button::setStringColor(const sf::Color &color)
{
	m_string.setFillColor(color);
}

void Button::setStringStyle(const sf::Text::Style &style)
{
	m_string.setStyle(style);
}

void Button::draw(sf::RenderWindow &window)
{
	window.draw(m_shape);
	window.draw(m_string);
}
