#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <ENGINE\Engine.hpp>
#include <SFML\Graphics.hpp>
#include <ENGINE\Graphics\Line.hpp>

#include <string>

class BaseEntity
{
public:
	BaseEntity();
	~BaseEntity();

	sf::Sprite sprite;
	sf::CircleShape moveDest;

	int id;
	int team = 0;
	int armor = 0;
	int hitpoints = 0;

	bool isSelected;
	bool movable;
	bool moving = false;

	const std::string type = "BaseEntity";

	void moveTo(const sf::Vector2f &dest);
	void Update();

	void setPosition(const sf::Vector2f& pos) { sprite.setPosition(pos); }
	sf::Vector2f getPosition() { return sprite.getPosition(); }

	sf::Vector2f movePos;
	sf::Texture texture;
};

#endif // !BASE_ENTITY_HPP
