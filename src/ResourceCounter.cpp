 #include "ResourceCounter.hpp"

#include "Util/Logger.hpp"

ResourceCounter::ResourceCounter(std::string iconname, const sf::Vector2f& position, resourceAmount_t count, resourceAmount_t max) : currentCount(count), maxCount(max)
{
	background.setFillColor(sf::Color(70, 70, 70));
	background.setSize(sf::Vector2f(46 - padding, 26 - padding));
	background.setOutlineThickness(padding);
	background.setOutlineColor(sf::Color(50, 50, 50));
	background.setPosition(position);

	iconTexture.loadFromFile("./bobwars/resource/textures/silk/" + iconname);
	icon.setTexture(&iconTexture);
	icon.setSize(sf::Vector2f(16, 16));
	icon.setPosition(sf::Vector2f(background.getPosition().x + padding, background.getPosition().y + padding));

	arial.loadFromFile("./bobwars/resource/interface/tahoma.ttf");

	text.setFont(arial);
	text.setCharacterSize(22);

	if (max == -1)
		text.setString(std::to_string(count));
	else
		text.setString(std::to_string(count) + "/" + std::to_string(max));

	text.setPosition(sf::Vector2f(icon.getPosition().x + icon.getGlobalBounds().width + padding, background.getPosition().y - 2));
	
	background.setSize(sf::Vector2f(text.getGlobalBounds().width + icon.getGlobalBounds().width + (padding * 4), background.getSize().y));
}

void ResourceCounter::setCount(resourceAmount_t count)
{
	currentCount = count;

	if (maxCount == -1)
		text.setString(std::to_string(currentCount));
	else
	{
		if (count >= maxCount)
			text.setFillColor(sf::Color::Red);
		else
			text.setFillColor(sf::Color::White);

		text.setString(std::to_string(currentCount) + "/" + std::to_string(maxCount));
	}

	background.setSize(sf::Vector2f(text.getGlobalBounds().width + icon.getGlobalBounds().width + (padding * 4), background.getSize().y));
}

void ResourceCounter::add(resourceAmount_t amount)
{
	logger::INFO("Current: " + std::to_string(currentCount));
	logger::INFO("Add: " + std::to_string(amount));
	logger::INFO("New count: " + std::to_string(currentCount + amount));
	resourceAmount_t newAmount = currentCount + amount;
	setCount(newAmount);
}

void ResourceCounter::take(resourceAmount_t amount)
{
	setCount(currentCount - amount);
}

void ResourceCounter::setMax(resourceAmount_t max)
{
	maxCount = max;
	text.setString(std::to_string(currentCount) + "/" + std::to_string(max));

	background.setSize(sf::Vector2f(text.getGlobalBounds().width + icon.getGlobalBounds().width + (padding * 4), background.getSize().y));
}

void ResourceCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(background);
	target.draw(icon);
	target.draw(text);
}
