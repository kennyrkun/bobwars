#ifndef COMPONENT_ENTITY_HPP
#define COMPONENT_ENTITY_HPP

#include "BaseEntity.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

class AppEngine;
class EntityComponent;

class ComponentEntity : public BaseEntity
{
public:
	ComponentEntity(int entityID, AppEngine* app = nullptr) : BaseEntity(entityID, false, false, false, false, true, EntityType::ComponentEntity, app) {}
	~ComponentEntity();

	EntityComponent* addComponent(EntityComponent* component);
	EntityComponent* removeComponent(EntityComponent* component);
	void 			 destroyComponent(EntityComponent* component);

	// Beware: this function is likely quite slow, due to multiple typecasts.
	template <typename T>
	T getComponent()
	{
		for (auto& [name, component] : components)
			if (dynamic_cast<T>(component))
				return static_cast<T>(component);

		return nullptr;
	}

	// TODO: this doesn't work
	EntityComponent* hasComponent(const std::string& componentName);
	// TODO: this doesn't work
	EntityComponent* getComponent(const std::string& componentName);

	virtual void Frame(float delta) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::map<const std::string, EntityComponent*> components;
};

#endif // !COMPONENT_ENTITY_HPP

