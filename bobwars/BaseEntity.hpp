#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <ENGINE\Engine.hpp>
#include <SFML\Graphics.hpp>
#include <ENGINE\Graphics\Line.hpp>

enum EntType
{
	bob,
	bobSpawn,
	goog,
	googSpawn
};

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

	bool movable;
	bool moving = false;
	void moveTo(const sf::Vector2f &dest);

	void Update();

	EntType type;

	bool isSelected;

private:
	sf::Vector2f movePos;
	sf::Texture texture;
};

#endif // !BASE_ENTITY_HPP
