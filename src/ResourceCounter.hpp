#ifndef RESOURCE_COUNTER_HPP
#define RESOURCE_COUNTER_HPP

#include <SFML/Graphics.hpp>

class ResourceCounter : public sf::Drawable
{
public:
	ResourceCounter(std::string icon, const sf::Vector2f& position, size_t count, size_t max = -1);

	void setCount(size_t count);
	void setMax(size_t max);

	sf::FloatRect getGlobalBounds() { return background.getGlobalBounds(); }

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	size_t currentCount;
	size_t maxCount; // -1 means no max

	// TODO: get this from resource manager
	sf::Font arial;
	sf::Text text;

private:
	sf::RectangleShape icon;
	sf::Texture iconTexture;
	sf::RectangleShape background;

	const float padding = 3.0f;
};

#endif // !RESOURCE_COUNTER_HPP