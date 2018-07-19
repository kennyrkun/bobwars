#include "BaseEntity.hpp"

#include "Util/Graphics/Line.hpp"
#include "Util/Util.hpp"

BaseEntity::BaseEntity()
{
	logger::INFO("Entity base class constructed.");

	moveDest.setRadius(5);
	moveDest.setPointCount(8);
	moveDest.setFillColor(sf::Color::Red);
	moveDest.setOrigin(sf::Vector2f(moveDest.getLocalBounds().width / 2, moveDest.getLocalBounds().height / 2));
}

BaseEntity::BaseEntity(int entityID, int team, int armour, int hitpoints, int health) : entityID(entityID), team(team), armor(armour), hitpoints(hitpoints), health(health)
{
	logger::INFO("Entity base class constructed.");

	moveDest.setRadius(5);
	moveDest.setPointCount(8);
	moveDest.setFillColor(sf::Color::Red);
	moveDest.setOrigin(sf::Vector2f(moveDest.getLocalBounds().width / 2, moveDest.getLocalBounds().height / 2));
}

BaseEntity::~BaseEntity()
{
	logger::DEBUG("Entity base class deconstructed.", true);
}

void BaseEntity::moveTo(const sf::Vector2f &pos)
{
	if (isMovable)
	{
		logger::INFO("Moving to X: " + std::to_string(pos.x) + ", Y: " + std::to_string(pos.y) + ". (" + std::to_string(entityID) + ")");
		movePos = pos;
		moveDest.setPosition(pos);
		isMoving = true;
	}
	else
	{
		logger::INFO("This entity is not isMovable! (" + type + ")");
	}
}

void BaseEntity::HandleEvents(const sf::Event & event)
{
}

void BaseEntity::Update()
{
	// TODO: stop entities from isMoving into each other

	if (isMoving)
	{
		int sX = static_cast<int>(sprite.getPosition().x);
		int sY = static_cast<int>(sprite.getPosition().y);
		int gX = static_cast<int>(movePos.x);
		int gY = static_cast<int>(movePos.y);

		if (sX > gX)
			sprite.move(-1.f, 0);
		else if (sX < gX)
			sprite.move(1.f, 0);

		if (sY > gY)
			sprite.move(0, -1.f);
		else if (sY < gY)
			sprite.move(0, 1.f);

		if (sX == gX && sY == gY)
		{
			logger::INFO("Done moving! (" + std::to_string(entityID) + ")");

			isMoving = false;
		}
		else
		{
			//line.setPoints(sprite.getPosition(), moveDest.getPosition());
		}
	}
}

void BaseEntity::Draw()
{
}

void BaseEntity::setPosition(const sf::Vector2f& pos)
{
	sprite.setPosition(pos);
}

sf::Vector2f BaseEntity::getPosition()
{
	return sprite.getPosition();
}
