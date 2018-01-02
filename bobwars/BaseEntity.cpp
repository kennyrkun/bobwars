#include "BaseEntity.hpp"

#include <ENGINE/Graphics/Line.hpp>
#include <ENGINE/Engine.hpp>

BaseEntity::BaseEntity()
{
	logger::INFO("Entity base class constructed.");

	moveDest.setRadius(5);
	moveDest.setPointCount(8);
	moveDest.setFillColor(sf::Color::Red);
	moveDest.setOrigin(sf::Vector2f(moveDest.getLocalBounds().width / 2, moveDest.getLocalBounds().height / 2));
}

BaseEntity::~BaseEntity()
{
	logger::SILENT("DEBUG", "Entity base class deconstructed.");
}

void BaseEntity::moveTo(const sf::Vector2f &pos)
{
	if (movable)
	{
		logger::INFO("Moving to X: " + std::to_string(pos.x) + ", Y: " + std::to_string(pos.y) + ". (" + std::to_string(id) + ")");
		movePos = pos;
		moveDest.setPosition(pos);
		moving = true;
	}
	else
	{
		logger::INFO("This entity is not movable! (" + type + ")");
	}
}

void BaseEntity::Update()
{
	if (moving && movable)
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
			logger::INFO("Done moving! (" + std::to_string(id) + ")");

			moving = false;
		}
		else
		{
			//line.setPoints(sprite.getPosition(), moveDest.getPosition());
		}
	}
}
