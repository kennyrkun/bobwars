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
		marker.setRadius(2);
		marker.setOrigin(sf::Vector2f(marker.getGlobalBounds().width / 2, marker.getGlobalBounds().height / 2));
	}

	bool getActive() const;

	void setMoveDestination(const sf::Vector2f destination);
	void cancelMovement();

	void Frame(float delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	sf::Vector2f destination;
	bool moveInProgress = false;

	sf::CircleShape marker;
};

#endif // !GROUND_MOVE_COMPONENT_HPP