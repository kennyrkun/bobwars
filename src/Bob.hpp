#ifndef BOB_HPP
#define BOB_HPP

#include "BaseEntity.hpp"
#include "Util/Graphics/Line.hpp"

class EntityManager;

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

class DrawConnectionsComponent : public EntityComponent
{
public:
	DrawConnectionsComponent(EntityManager* entman) : entMan(entman)
	{
		name = "DrawConnections";
	}

	void Frame(float delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	EntityManager* entMan;
	std::vector<Line> lines;
};

class Bob : public ComponentEntity
{
public:
	Bob(const int entityID);
	~Bob();
};

#endif // !BOB_HPP
