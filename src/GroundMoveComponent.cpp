#include "GroundMoveComponent.hpp"

#include "Util/Logger.hpp"

bool GroundMoveComponent::getActive() const
{
    return moveInProgress;
}

void GroundMoveComponent::setMoveDestination(const sf::Vector2f destination)
{
	marker.setPosition(destination);
	
	this->destination = destination;
	moveInProgress = true;
}

const sf::Vector2f& GroundMoveComponent::getMoveDesintation() const
{
	return destination;
}

void GroundMoveComponent::cancelMovement()
{
	moveInProgress = false;
}

void GroundMoveComponent::Frame(float delta)
{
//	linear interpolation: c * t / d + b

	if (moveInProgress)
	{
		int sX = static_cast<int>(owner->sprite.getPosition().x);
		int sY = static_cast<int>(owner->sprite.getPosition().y);
		int gX = static_cast<int>(destination.x);
		int gY = static_cast<int>(destination.y);

		if (sX > gX)
			owner->sprite.move(-1.f, 0);
		else if (sX < gX)
			owner->sprite.move(1.f, 0);

		if (sY > gY)
			owner->sprite.move(0, -1.f);
		else if (sY < gY)
			owner->sprite.move(0, 1.f);

		if (sX == gX && sY == gY)
		{
			logger::INFO("Done moving! (" + std::to_string(owner->entityID) + ")");
			moveInProgress = false;
		}
	}
}

void GroundMoveComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (moveInProgress)
		target.draw(marker, states);
}