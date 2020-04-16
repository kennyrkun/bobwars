#include "SpriteButtonWithTooltip.hpp"

#include <SFUI/Theme.hpp>

// TODO: fix button not showing unpressed state

SpriteButtonWithTooltip::SpriteButton(const sf::Texture& texture, const sf::String& string) : m_pressed(false)
{
	size_t width = texture.getSize().x;
	size_t height = texture.getSize().y / 4; // default, hover, pressed, focus

	m_background.setTexture(texture);
	m_background.setTextureRect(sf::IntRect(0, 0, width, height));

	setSize(sf::Vector2f(width, height));

	m_text.setFont(SFUI::Theme::getFont());
	m_text.setCharacterSize(SFUI::Theme::textCharacterSize);

	setString(string);
}

void SpriteButtonWithTooltip::setString(const sf::String& string)
{
	m_text.setString(string);
	centerText(m_text);
}

const sf::String& SpriteButtonWithTooltip::getString() const
{
	return m_text.getString();
}

void SpriteButtonWithTooltip::setFont(const sf::Font& font)
{
	m_text.setFont(font);
	centerText(m_text);
}

const sf::Font& SpriteButtonWithTooltip::getFont() const
{
	return *m_text.getFont();
}

void SpriteButtonWithTooltip::setTextSize(size_t size)
{
	m_text.setCharacterSize(size);
	centerText(m_text);
}

void SpriteButtonWithTooltip::setTexture(const sf::Texture & texture)
{
	size_t width = texture.getSize().x;
	size_t height = texture.getSize().y / 4; // default, hover, pressed, focus

	m_background.setTexture(texture);
	m_background.setTextureRect(sf::IntRect(0, 0, width, height));

	setSize(sf::Vector2f(width, height));
}

void SpriteButtonWithTooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_background, states);
	target.draw(m_text, states);
}

// Callbacks

void SpriteButtonWithTooltip::onStateChanged(State state)
{
	sf::Vector2f size = getSize();
	switch (state)
	{
	case State::Default:
		m_background.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
		break;
	case State::Hovered:
		m_background.setTextureRect(sf::IntRect(0, size.y, size.x, size.y));
		break;
	case State::Pressed:
		m_background.setTextureRect(sf::IntRect(0, size.y * 2, size.x, size.y));
		break;
	case State::Focused:
		m_background.setTextureRect(sf::IntRect(0, size.y * 3, size.x, size.y));
		break;
	}
}

void SpriteButtonWithTooltip::onMouseMoved(float x, float y)
{
	if (isFocused())
	{
		if (containsPoint({ x, y }) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
			press();
		else
			release();
	}

	if (getState() == State::Pressed)
	{
		if (containsPoint({ x, y })) // TODO: check if this should check if left mouse button is pressed
			press();
		else
			release();
	}
}

void SpriteButtonWithTooltip::onMousePressed(float, float)
{
	press();
}

void SpriteButtonWithTooltip::onMouseReleased(float x, float y)
{
	release();

	if (containsPoint({x, y}))
		triggerCallback();
}

void SpriteButtonWithTooltip::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Return)
	{
		press();
		triggerCallback();
		// HACK: we don't have to do this in onMousePressed, why do we have to do it here?
		onStateChanged(State::Pressed);
	}
}

void SpriteButtonWithTooltip::onKeyReleased(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Return)
	{
		release();
		// HACK: we don't have to do this in onMousePressed, why do we have to do it here?
		onStateChanged(State::Focused);
	}
}

void SpriteButtonWithTooltip::press()
{
	if (!m_pressed)
	{
		m_pressed = true;
		m_text.move(0, 1);
	}
}

void SpriteButtonWithTooltip::release()
{
	if (m_pressed)
	{
		m_pressed = false;
		m_text.move(0, -1);
	}
}

void SpriteButtonWithTooltip::centerText(sf::Text& text)
{
	sf::FloatRect t = text.getLocalBounds();
	sf::FloatRect b = m_background.getLocalBounds();

	text.setPosition(int(b.width - t.width) / 2, (int(b.height - t.height) / 2) - 4);
}
