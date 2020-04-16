#ifndef GROUND_MOVE_COMPONENT_HPP
#define GROUND_MOVE_COMPONENT_HPP

#include "EntityComponent.hpp"

#include <SFML/Graphics.hpp>

class GroundMoveComponent : public EntityComponent
{
public:
	GroundMoveComponent()
	{
		name = "GroundMove";
	}

	bool getActive() const;

	void setMoveDestination(const sf::Vector2f destination);
	void cancelMovement();

	void Frame(float delta) override;

private:
	sf::Vector2f destination;
	bool moveInProgress = false;
};

#endif // !GROUND_MOVE_COMPONENT_HPP