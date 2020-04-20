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
	inline T getComponent()
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

	/*
	inline const std::vector<const std::string> componentList() const
	{
		std::string compon = "";

		for (auto& [name, component] : components)
			compon += name + ",";

		return compon;
	}
	*/

	// Note: It is important that any entity which overrides this method call down to ComponentEntity::Frame(delta)
	// If the derived entity fails to do this, no components will be updated.
	virtual void Frame(float delta) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::map<const std::string, EntityComponent*> components;
};

#endif // !COMPONENT_ENTITY_HPP

