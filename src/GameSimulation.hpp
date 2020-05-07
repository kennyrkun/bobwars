#ifndef GAME_SIMULATION_HPP
#define GAME_SIMULATION_HPP

#include "AppEngine.hpp"
#include "EntityManager.hpp"
#include "GameWorld.hpp"
#include "Interface.hpp"
#include "BaseEntity.hpp"

#include <SFML/Graphics.hpp>

#include <vector>

struct Resources
{
    int memes = 100;
};

class GameSimulation : public sf::Drawable
{
public:
    GameSimulation();

    void create(AppEngine* app);

    AppEngine* app;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);

    sf::Clock googleTimer;
	sf::Clock resourceTimer;
	sf::Clock gameClock; // total time of the game

	EntityManager *entMan;
	Interface *ui;

    GameWorld world;

	void createEntity(EntityType type, const sf::Vector2f& position);
	void deleteEntities(const std::vector<BaseEntity*>& entities);

	void deleteButton();

/*
	enum UnitCommands
    {
        CreateUnit,
        DeleteUnit,
        MoveUnit,
        UnitAction,

        FailNotEnoughResource,
        FailTooManyUnits,
    };
*/

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // !GAME_SIMULATION_HPP