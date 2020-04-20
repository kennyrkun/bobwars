#ifndef BOB_HPP
#define BOB_HPP

#include "ComponentEntity.hpp"
#include "GroundMoveComponent.hpp"
#include "EntityManager.hpp"

#include "Util/Graphics/Line.hpp"

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
	Bob(const int entityID, EntityManager* entities);
	~Bob();

	Status addTask(EntityType type) override;

private:
	EntityManager* entities;
};

#endif // !BOB_HPP
