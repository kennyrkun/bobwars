#include "BaseEntity.hpp"

#include "Util/Graphics/Line.hpp"
#include "Util/Util.hpp"

BaseEntity::BaseEntity(const int entityID, const bool isBuilding, const bool isLand, const bool isSea, const bool isRanged, const bool isComponentEntity, EntityType type, AppEngine* app) :
						entityID(entityID), isBuilding(isBuilding), isLand(isLand), isSea(isSea), isRanged(isRanged), isComponentEntity(isComponentEntity), typeEnum(type), app(app)
{
	logger::DEBUG("Entity base class constructed.");

	this->type = "baseentity";
}

BaseEntity::~BaseEntity()
{
	logger::DEBUG("Entity base class deconstructed.", true);
}

void BaseEntity::setPosition(const sf::Vector2f& pos)
{
	sprite.setPosition(pos);
}

sf::Vector2f BaseEntity::getPosition() const
{
	return sprite.getPosition();
}

void BaseEntity::onDamage(float amount, BaseEntity* entity, DamageCause cause)
{
	health -= amount;

	if (health <= 0)
		logger::INFO(type + " was killed by " + entity->type);
}

void BaseEntity::Frame(float delta)
{
}

void BaseEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}
