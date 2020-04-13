#include "BaseEntity.hpp"

#include "Util/Graphics/Line.hpp"
#include "Util/Util.hpp"

BaseEntity::BaseEntity(const int entityID, const bool isBuilding, const bool isLand, const bool isSea, const bool isRanged, const bool isComponentEntity, EntityType type, AppEngine* app) :
						entityID(entityID), isBuilding(isBuilding), isLand(isLand), isSea(isSea), isRanged(isRanged), isComponentEntity(isComponentEntity), typeEnum(type), app(app)
{
	logger::INFO("Entity base class constructed.");

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
		logger::INFO("this entity is now FUCKING DEAD");
}

void BaseEntity::Frame(float delta)
{
}

void BaseEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void BuildingEntity::setGarrisonPoint(const sf::Vector2f& point)
{
	hasGarrisonPoint = true;
	garrisonPoint = point;

	logger::INFO("Set garrison point to " + std::to_string(point.x) + ", " + std::to_string(point.y));
}

EntityComponent* ComponentEntity::addComponent(EntityComponent* component)
{
	component->owner = this;
	components.emplace(component->name, component);
}

EntityComponent* ComponentEntity::removeComponent(EntityComponent* component)
{
	component->owner = nullptr;
	components.erase(component->name);
	return component;
}

void ComponentEntity::destroyComponent(EntityComponent* component)
{
	component->owner = nullptr;
	components.erase(component->name);
	delete component;
}

EntityComponent* ComponentEntity::hasComponent(const std::string& componentName)
{
	if (components.find(componentName) != components.end())
		return components.at(componentName);
	else
		return nullptr;
}

EntityComponent* ComponentEntity::getComponent(const std::string& componentName)
{
	return hasComponent(componentName);
}

void ComponentEntity::Frame(float delta)
{
	for (auto& [name, component] : components)
		component->Frame(delta);
}

void ComponentEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& [name, component] : components)
		target.draw(*component);

	BaseEntity::draw(target, states);
}

