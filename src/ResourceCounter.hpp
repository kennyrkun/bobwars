#ifndef RESOURCE_COUNTER_HPP
#define RESOURCE_COUNTER_HPP

#include <SFML/Graphics.hpp>

typedef signed long long int resourceAmount_t;

class ResourceCounter : public sf::Drawable
{
public:
	ResourceCounter(std::string icon, const sf::Vector2f& position, resourceAmount_t count = 0, resourceAmount_t max = -1);

	void setCount(resourceAmount_t count);
	void add(resourceAmount_t amount);
	void take(resourceAmount_t amount);
	void setMax(resourceAmount_t max);

	sf::FloatRect getGlobalBounds() { return background.getGlobalBounds(); }

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	resourceAmount_t currentCount;
	resourceAmount_t maxCount; // -1 means no max

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