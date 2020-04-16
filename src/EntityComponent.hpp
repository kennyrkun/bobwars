#ifndef ENTITY_COMPONENT_HPP
#define ENTITY_COMPONENT_HPP

#include "ComponentEntity.hpp"

#include <SFML/Graphics.hpp>

class EntityComponent : public sf::Drawable
{
public:
	ComponentEntity* owner = nullptr;

	virtual void Frame(float delta) {}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}

	std::string name;
};

#endif // !ENTITY_COMPONENT_HPP
