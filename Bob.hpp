#ifndef BOB_HPP
#define BOB_HPP

#include "BaseEntity.hpp"

class GroundMoveComponent : public EntityComponent
{
public:
	GroundMoveComponent()
	{
		name = "GroundMove";
	}

	void setDestination(const sf::Vector2f destination);
	void cancelMovement();

	void Frame(float delta) override;

private:
	sf::Vector2f destination;
	bool moveInProgress = false;
};

class Bob : public ComponentEntity
{
public:
	Bob(const int entityID);
	~Bob();
};

#endif // !BOB_HPP
