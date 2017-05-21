#include "BaseEntity.hpp"

BaseEntity::BaseEntity()
{
	logger::SILENT("DEBUG", "Entity base class constructed.");

	m_texture.loadFromFile("resource\\textures\\bob.png");
	m_sprite.setTexture(m_texture);

	m_sprite.setOrigin(sf::Vector2f(m_sprite.getLocalBounds().width / 2, m_sprite.getLocalBounds().height / 2));

	move_dest.setRadius(5);
	move_dest.setFillColor(sf::Color::Red);
}

BaseEntity::~BaseEntity()
{
	logger::SILENT("DEBUG", "Entity base class deconstructed.");
}

void BaseEntity::moveTo(const sf::Vector2f &pos)
{
	logger::INFO("Moving to X: " + std::to_string(pos.x) + ", Y: " + std::to_string(pos.y) + ". (" + std::to_string(this->m_id) + ")");
	movePos = pos;
	move_dest.setPosition(pos);
	m_moving = true;
}

void BaseEntity::Update()
{
	if (m_moving)
	{
		int sX = m_sprite.getPosition().x;
		int sY = m_sprite.getPosition().y;
		int gX = movePos.x;
		int gY = movePos.y;

		if (sX > gX)
			m_sprite.move(-.1f, 0);
		else if (sX < gX)
			m_sprite.move(.1f, 0);

		if (sY > gY)
			m_sprite.move(0, -.1f);
		else if (sY < gY)
			m_sprite.move(0, .1f);

		if (sX == gX && sY == gY)
		{
			logger::INFO("Done moving! (" + std::to_string(this->m_id) + ")");

			m_moving = false;
		}
	}
}