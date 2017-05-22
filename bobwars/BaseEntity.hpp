#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <ENGINE\Engine.hpp>

class BaseEntity
{
public:
	sf::CircleShape moveDest;
	sf::Sprite sprite;
	int id;
	int team;
	int armor;
	int hitpoints;
	bool moving;

	BaseEntity();
	~BaseEntity();

	void moveTo(const sf::Vector2f &dest);
	void Update();

private:
	sf::Vector2f movePos;
	sf::Texture texture;
};

#endif // BASE_ENTITY_HPP
