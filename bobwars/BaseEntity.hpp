#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <SFML/Graphics.hpp>

#include <string>

// I have a feeling the entire entity system has a design of complete horseshit.
// If anyone would like to fix it, it'd be very much appreciated.

class BaseEntity
{
public:
	BaseEntity();
	~BaseEntity();

	sf::Sprite sprite;

	//TODO: move this out of the entity class itself and have the renderer draw one for each bob, since this is just used in debug mode.
	sf::CircleShape moveDest;

	int id;

	int team;
	int armor;
	int hitpoints;
	int health;

	bool isSelected;
	bool movable;
	bool moving;

	std::string type;

	void moveTo(const sf::Vector2f &dest);
	void Update();

	void setPosition(const sf::Vector2f& pos) { sprite.setPosition(pos); }
	sf::Vector2f getPosition() { return sprite.getPosition(); }

	sf::Vector2f movePos;
	sf::Texture texture;
};

#endif // !BASE_ENTITY_HPP
