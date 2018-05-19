#include "Bob.hpp"

#include "Util/Logger.hpp"

Bob::Bob()
{
	logger::INFO("Bob entity created.");

	texture.loadFromFile("bobwars/resource/textures/bob.png"); // no point in logging failure, as SFML does this for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	moveDest.setRadius(5);
	moveDest.setPointCount(8);
	moveDest.setFillColor(sf::Color::Red);
	moveDest.setOrigin(sf::Vector2f(moveDest.getLocalBounds().width / 2, moveDest.getLocalBounds().height / 2));
	
	team = 1;
	health = 10;
	armor = 0;
	hitpoints = 1;
	type = "bob";
	movable = true;
}

Bob::~Bob()
{
}
