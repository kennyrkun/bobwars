#include "GameWorld.hpp"

#include "Util/Logger.hpp"

void GameWorld::generate()
{
    logger::INFO("Generating map...");

    if (!mapTexture.loadFromFile("./bobwars/resource/textures/world.png"))
        abort();

    map.setSize(sf::Vector2f(800, 600));
	map.setOrigin(sf::Vector2f(map.getSize().x / 2, map.getSize().y / 2));
	map.setPosition(sf::Vector2f(0, 0));
    map.setTexture(&mapTexture);

	//TODO: make camera align with world center on game start
}

void GameWorld::loadFromFile(const std::string& filename)
{
    logger::WARNING("Map loading not implemented.");
}

void GameWorld::saveToFile(const std::string& filename)
{
    logger::WARNING("Map saving not implemented.");
}

const sf::Vector2f& GameWorld::getCenter() const
{
    return sf::Vector2f(map.getSize().x / 2, map.getSize().y / 2);
}

const sf::Vector2f& GameWorld::getSize() const
{
    return map.getSize();
}

const sf::FloatRect GameWorld::getGlobalBounds() const
{
    return map.getGlobalBounds();
}

void GameWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(map);
}