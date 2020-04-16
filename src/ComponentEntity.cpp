#include "ComponentEntity.hpp"

#include "EntityComponent.hpp"

ComponentEntity::~ComponentEntity()
{
	for (auto& [name, component] : components)
		delete component;
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
