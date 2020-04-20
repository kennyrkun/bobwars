#include "Explosion.hpp"

#include "Util/Logger.hpp"
#include "Util/Util.hpp"

float InterpolateEaseOut(float t, float b, float c, float d)
{
	return c * ((t = t / d - 1) * t * t * t * t + 1) + b;
}

Explosion::Explosion(const int entityID, EntityManager* app, const sf::Vector2f& position, float radius, float damage, float duration) : 
    damage(damage), duration(duration), entities(app), 
    BaseEntity(entityID, false, true, true, true, false, EntityType::Explosion)
{
    logger::DEBUG("Explosion entity created.");

	team = Team::Neutral;
	maxHealth = 1;
	health = maxHealth;
	hitpoints = damage;
	type = "explosion";

    shape.setRadius(radius);
    shape.setOrigin(sf::Vector2f(shape.getGlobalBounds().width / 2, shape.getGlobalBounds().height / 2));
    shape.setPosition(position);

    originalRadius = radius;
    changeInRadius = 0 - radius;

    texture.loadFromFile("./bobwars/resource/textures/explosion.png");
    shape.setTexture(&texture);

    buffer.loadFromFile("./bobwars/resource/audio/explosion.wav");
    sound.setBuffer(buffer);
    sound.play();

    timer.restart();
}

void Explosion::onDamage(float amount, BaseEntity* entity, DamageCause cause)
{
    logger::INFO("Explosion killed by " + entity->type);
}

void Explosion::Frame(const float delta)
{
	for (BaseEntity* entity : entities->getNearbyEntities(this, shape.getRadius()))
	{
        if (entity == this)
            logger::ERROR("Explosion will target itself!");

        float dist = util::distance(getPosition(), entity->getPosition());

        if (dist <= shape.getRadius())
            entity->onDamage(damage, this);
	}

    int newSize = InterpolateEaseOut(timer.getElapsedTime().asMilliseconds(), originalRadius, changeInRadius, duration * 1000);
    int sizeDiff = shape.getRadius() - newSize;
    shape.setRadius(newSize);
    shape.move(sf::Vector2f(sizeDiff, sizeDiff));


    if (timer.getElapsedTime().asSeconds() > duration)
    {
        logger::DEBUG("Explosion is finished.");
        health = -1;
    }
}

void Explosion::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
}