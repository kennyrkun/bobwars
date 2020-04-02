#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <map>

// I have a feeling the entire entity system has a design of complete horseshit.
// If anyone would like to fix it, it'd be very much appreciated. (:

enum class EntityType
{
	BaseEntity,
	ComponentEntity,
	Bob,
	CommentSection,
	GooglePlus, 
	Google
};

class TransformableEntity
{
public:
	virtual void setPosition(const sf::Vector2f& newPosition)
	{
		position = newPosition;
	}

	virtual const sf::Vector2f& getPosition()
	{
		return position;
	}

private:
	sf::Vector2f position;
};

class MoveableEntity
{

};

class BaseEntity : public sf::Drawable
{
public:
	BaseEntity(const int entityID, const bool isBuilding, const bool isLand, const bool isSea, const bool isRanged, const bool isMovable,
				EntityType type);
	~BaseEntity();

	const int entityID = -1;

	int team = -1;
	int armor = -1;
	int health = -1, maxHealth = -1;
	int hitpoints = -1;
	float rateOfFire = -1;

	bool isSelected = false;

	const bool isBuilding = false;
	const bool isLand = false;
	const bool isSea = false;
	const bool isRanged = false;
	const bool isMovable = false;
	bool isMoving = false;

	void setGarrisonPoint(const sf::Vector2f& point);

	sf::Vector2f garrisonPoint;
	bool hasGarrisonPoint;

	std::string type;
	const EntityType typeEnum = EntityType::BaseEntity;

	sf::Vector2f movePos;
	sf::CircleShape moveDest; // TODO: move this out of the entity class itself and have the renderer draw one for each ent, since this is just used in debug mode.

	sf::Sprite sprite;
	sf::Texture texture;

	virtual void moveTo(const sf::Vector2f &dest);

	virtual void setPosition(const sf::Vector2f& pos);
	virtual sf::Vector2f getPosition();

	virtual void onCollide(BaseEntity* other) {};

	virtual void Frame(float delta);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // !BASE_ENTITY_HPP
