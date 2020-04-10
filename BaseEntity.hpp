#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include "AppEngine.hpp"

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <vector>

// this is bad somebody fix

enum class EntityType
{
	BaseEntity,
	ComponentEntity,
	BuildingEntity,
	InteractableObject,
	Bob,
	CommentSection,
	Google,
	GooglePlus,
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

	float maxInteractDistance = 25.0f;

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
	const bool isComponentEntity = false;

	std::string type;
	const EntityType typeEnum = EntityType::BaseEntity;

	sf::Sprite sprite;
	sf::Texture texture;

	std::vector<EntityAction> actions;

	virtual void setPosition(const sf::Vector2f& pos);
	virtual sf::Vector2f getPosition() const;

	// returns: false if the interaction failed
	//			true if the interaction succeeded
	virtual bool onInteract(BaseEntity* entity) {}

	virtual void Frame(float delta);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

class BuildingEntity : public BaseEntity
{
public:
	BuildingEntity(const int entityID, AppEngine* app = nullptr) : BaseEntity(entityID, true, true, false, false, false, EntityType::BuildingEntity, app)
	{
	}

	void setGarrisonPoint(const sf::Vector2f& point);
	bool hasGarrisonPoint;
	sf::Vector2f garrisonPoint;
};

class ComponentEntity;

class EntityComponent : public sf::Drawable
{
public:
	ComponentEntity* owner = nullptr;

	virtual void Frame(float delta) {}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}

	std::string name;
};

class ComponentEntity : public BaseEntity
{
public:
	ComponentEntity(int entityID, AppEngine* app = nullptr) : BaseEntity(entityID, false, false, false, false, true, EntityType::ComponentEntity, app) {}
	~ComponentEntity()
	{
		for (auto& [name, component] : components)
			delete component;
	}

	EntityComponent* addComponent(EntityComponent* component);
	EntityComponent* removeComponent(EntityComponent* component);
	void destroyComponent(EntityComponent* component);

	/*
	// TODO: this doesn't work
	template <typename T>
	T* getComponent(const std::string& componentName)
	{
		if (components.find(componentName) != components.end())
		{
			T component* = dynamic_cast<T>(components.at(componentName));
			return component;
		}
		else
			return nullptr;
	}
	*/

	// TODO: this doesn't work
	EntityComponent* hasComponent(const std::string& componentName);
	// TODO: this doesn't work
	EntityComponent* getComponent(const std::string& componentName);

	void Frame(float delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::map<const std::string, EntityComponent*> components;
};

#endif // !BASE_ENTITY_HPP

