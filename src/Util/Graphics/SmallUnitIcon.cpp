#include "SmallUnitIcon.hpp"

SmallUnitIcon::SmallUnitIcon(BaseEntity* entity) : entity(entity)
{
	box.setSize(sf::Vector2f(30, 30));
	box.setTexture(&entity->texture);
	setSize(box.getSize());

	red.setFillColor(sf::Color::Red);
	red.setSize(sf::Vector2f(box.getSize().y, 4));
	red.setPosition(sf::Vector2f(box.getPosition().x, box.getPosition().y + (box.getSize().y - red.getSize().y)));

	healthBar.setWidth(box.getSize().x);
	healthBar.setHeight(red.getSize().y);
	healthBar.setPosition(red.getPosition());

	update();
}

SmallUnitIcon::~SmallUnitIcon()
{
}

void SmallUnitIcon::update()
{
	healthBar.setValue(entity->health, entity->maxHealth);
}

void SmallUnitIcon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(box, states);
	target.draw(red, states);
	target.draw(healthBar, states);
}
