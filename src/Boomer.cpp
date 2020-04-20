#include "Boomer.hpp"

#include "GroundMoveComponent.hpp"
#include "Util/Logger.hpp"
#include "Util/Util.hpp"

#include "Explosion.hpp"

Boomer::Boomer(int entityID, EntityManager* app) : entities(app), ComponentEntity(entityID)
{
    logger::INFO("Boomer entity created.");

	texture.loadFromFile("./bobwars/resource/textures/boomer.png"); // no point in logging failure, as SFML does this for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	team = Team::GoodGuys;
	maxHealth = 50;
	health = maxHealth;
	hitpoints = 1;
	type = "boomer";

	EntityAction detonateAction;
	detonateAction.name = "Detonate";
	detonateAction.description = "Detonates the Boomer.";
	detonateAction.hotkey = sf::Keyboard::Key::C;
	detonateAction.buttonTexture = "./bobwars/resources/textures/spritebuttons/boomer_detonate.png";
	actions.push_back(detonateAction);

	addComponent(new GroundMoveComponent);
}

void Boomer::Frame(const float delta)
{
    int entitiesInRange = 0;

	for (BaseEntity* entity : entities->getNearbyEntities(this, maxViewDistance))
	{
		if (entity->team != Team::BadGuys)
            continue;

        float dist = util::distance(getPosition(), entity->getPosition());

        if (dist <= maxViewDistance)
            entitiesInRange += 1;
	}

    const int requiredEntityCount = 1;

    if (entitiesInRange >= requiredEntityCount)
    {
        health = -1;

        Explosion*  explosion = new Explosion(entities->entities.size() + 1, entities, getPosition(), maxViewDistance, 100.0f, 3.0f);
        entities->addEnt(explosion);
    }
}