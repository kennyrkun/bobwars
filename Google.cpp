#include "Google.hpp"

#include "Util/Logger.hpp"

Google::Google(const int entityID) : BaseEntity(entityID, false, true, false, false, false, EntityType::Google)
{
	logger::INFO("Google entity created.");

	texture.loadFromFile("bobwars/resource/textures/google.png"); // no point in logging failure, as SFML does it for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	team = 1;
	maxHealth = 200;
	health = maxHealth;
	hitpoints = 1;
	type = "Google";
}

Google::~Google()
{
}
