#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <ENGINE\Engine.hpp>
#include <SFML\Graphics.hpp>
#include <ENGINE\Graphics\Line.hpp>

class BaseEntity
{
public:
	sf::Sprite sprite;
	sf::CircleShape moveDest;

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
