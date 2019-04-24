#include "Bob.hpp"

#include "Util/Logger.hpp"

Bob::Bob(const int entityID) : BaseEntity(entityID, false, true, false, false, true, EntityType::Bob)
{
	logger::INFO("Bob entity created.");

	texture.loadFromFile("bobwars/resource/textures/bob.png"); // no point in logging failure, as SFML does this for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	team = 1;
	health = 100;
	hitpoints = 1;
	type = "bob";
}

Bob::~Bob()
{
}
