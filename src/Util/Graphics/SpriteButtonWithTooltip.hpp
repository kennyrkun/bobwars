#ifndef SPRITE_BUTTON_WITH_TOOLTIP_HPP
#define SPRITE_BUTTON_WITH_TOOLTIP_HPP

#include <SFUI/Widget.hpp>

// TODO: why is this different than the other Buttons?
// TODO: SpriteButton doesn't release when mouse is moved out of bounds

// Button with custom spritesheet and text options.
class SpriteButtonWithToolTip : public SFUI::Widget
{
public:
	SpriteButton(const sf::Texture& texture, const sf::String& label = "");

	void setString(const sf::String& string);
	const sf::String& getString() const;

	void setFont(const sf::Font& font);
	const sf::Font& getFont() const;

	void setTextSize(size_t size);

	void setTexture(const sf::Texture& texture);

	void onStateChanged(State state);
	void onMouseMoved(float x, float y);
	void onMousePressed(float x, float y);
	void onMouseReleased(float x, float y);
	void onKeyPressed(sf::Keyboard::Key key);
	void onKeyReleased(sf::Keyboard::Key key);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void press();
	void release();

	sf::Text   m_text;
	sf::Sprite m_background;
	bool       m_pressed;

	void centerText(sf::Text& text);
};

#endif // !SPRITE_BUTTON_WITH_TOOLTIP_HPP
