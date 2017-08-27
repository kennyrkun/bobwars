#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <ENGINE\Engine.hpp>
#include <SFML\Graphics.hpp>
#include <ENGINE\Graphics\Line.hpp>

class BaseEntity
{
public:
	BaseEntity();
	~BaseEntity();

	sf::Sprite sprite;
	sf::CircleShape moveDest;

	int id;
	int team;
	int armor;
	int hitpoints;
	bool moving = false;

	void moveTo(const sf::Vector2f &dest);
	void Update();

private:
	sf::Vector2f movePos;
	sf::Texture texture;
};

#endif // BASE_ENTITY_HPP
