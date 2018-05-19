#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <SFML/Graphics.hpp>

#include <string>

// I have a feeling the entire entity system has a design of complete horseshit.
// If anyone would like to fix it, it'd be very much appreciated. (:

enum class EntityType
{
	BaseEntity,
	Bob,
	CommentSection,
	GooglePlus
};

class BaseEntity
{
public:
	BaseEntity();
	BaseEntity(int entityID, int team, int armour, int hitpoints, int health);
	~BaseEntity();

	sf::Sprite sprite;

	//TODO: move this out of the entity class itself and have the renderer draw one for each ent, since this is just used in debug mode.
	sf::CircleShape moveDest;

	int entityID;

	int team;
	int armor;
	int hitpoints;
	int health;

	bool isSelected;
	bool movable;
	bool moving;

	std::string type;

	virtual void moveTo(const sf::Vector2f &dest);

	virtual void HandleEvents(const sf::Event& event);
	virtual void Update(); // if moveTo is overriden this should be too
	virtual void Draw();

	virtual void setPosition(const sf::Vector2f& pos);
	virtual sf::Vector2f getPosition();

	sf::Vector2f movePos;
	sf::Texture texture;
};

#endif // !BASE_ENTITY_HPP
