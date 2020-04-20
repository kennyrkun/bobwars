 #ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include "AppEngine.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <vector>

// this is bad somebody fix

enum class Team
{
	BadGuys,
	GoodGuys,
	Neutral,
};

enum class EntityType
{
	BaseEntity,
	ComponentEntity,
	BuildingEntity,
	InteractableObject,
	Bob,
	Boomer,
	CommentSection,
	Google,
	GooglePlus,
	Explosion,
};

class EntityAction
{
public:
	std::string name;
	std::string description;
	sf::Keyboard::Key hotkey;

	int callbackID = -1;

	std::string buttonTexture;
};

class BaseEntity : public sf::Drawable
{
public:
	BaseEntity(const int entityID, const bool isBuilding, const bool isLand, const bool isSea, const bool isRanged, const bool isComponentEntity, EntityType type, AppEngine* app = nullptr);
	~BaseEntity();

	const int entityID;

	AppEngine* app;

	enum class Stance
	{
		Aggressive,
		Defensive,
		StandGround,
		NoAttack
	};

	float maxAttackDistance = 75.0f;
	float minAttackDistance = 0.0f;
	float maxViewDistance = 125.0f;

	float creationCost = 100;

	Team team = Team::Neutral;

	int armor = -1;
	int health = -1, maxHealth = -1;
	int hitpoints = -1;
	float rateOfFire = -1;

	bool isSelected = false;

	const bool isBuilding = false;
	const bool isLand = false;
	const bool isSea = false;
	const bool isRanged = false;
	const bool isComponentEntity = false;

	std::string type;
	const EntityType typeEnum = EntityType::BaseEntity;

	sf::Sprite sprite;
	sf::Texture texture;

	std::vector<EntityAction> actions;

	BaseEntity* targetEntity;

	virtual void setPosition(const sf::Vector2f& pos);
	virtual sf::Vector2f getPosition() const;

	// returns: false if the interaction failed
	//			true if the interaction succeeded
	virtual bool onInteract(BaseEntity* entity) {}

	enum class DamageCause
	{
		EnemyAttack,
		FriendlyAttack,
		NeutralAttack,
		WorldDamage,
		Complications,
		Unknown
	};

	virtual void onDamage(float amount, BaseEntity* entity, DamageCause cause = DamageCause::Unknown);

	virtual void Frame(const float delta);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif // !BASE_ENTITY_HPP

