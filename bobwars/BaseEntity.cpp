#include "BaseEntity.hpp"

BaseEntity::BaseEntity()
{
	logger::SILENT("DEBUG", "Entity base class constructed.");

	texture.loadFromFile("resource\\textures\\bob.png");
	sprite.setTexture(texture);

	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	moveDest.setRadius(5);
	moveDest.setFillColor(sf::Color::Red);
}

BaseEntity::~BaseEntity()
{
	logger::SILENT("DEBUG", "Entity base class deconstructed.");
}

void BaseEntity::moveTo(const sf::Vector2f &pos)
{
	logger::INFO("Moving to X: " + std::to_string(pos.x) + ", Y: " + std::to_string(pos.y) + ". (" + std::to_string(this->id) + ")");
	movePos = pos;
	moveDest.setPosition(pos);
	moving = true;
}

void BaseEntity::Update()
{
	if (moving)
	{
		int sX = sprite.getPosition().x;
		int sY = sprite.getPosition().y;
		int gX = movePos.x;
		int gY = movePos.y;

		if (sX > gX)
			sprite.move(-.1f, 0);
		else if (sX < gX)
			sprite.move(.1f, 0);

		if (sY > gY)
			sprite.move(0, -.1f);
		else if (sY < gY)
			sprite.move(0, .1f);

		if (sX == gX && sY == gY)
		{
			logger::INFO("Done moving! (" + std::to_string(id) + ")");

			moving = false;
		}
	}
}