#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "BaseEntity.hpp"
#include "EntityManager.hpp"

#include <SFML/Audio.hpp>

class Explosion : public BaseEntity
{
public:
    Explosion(const int entityID, EntityManager* app, const sf::Vector2f& position, float radius, float damage, float duration);

    void onDamage(float amount, BaseEntity* entity, DamageCause cause = DamageCause::Unknown);

    void Frame(const float delta) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::CircleShape shape;
    sf::Texture texture;

    sf::SoundBuffer buffer;
    sf::Sound sound;

    EntityManager* entities;

    const float damage;
    const float duration;

    float originalRadius;
    float changeInRadius;

    sf::Clock timer;
    sf::Time spawnTime;
};

#endif // !EXPLOSION_HPP