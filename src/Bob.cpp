#include "Bob.hpp"

#include "EntityManager.hpp"
#include "CommentSection.hpp"

#include "Util/Logger.hpp"
#include "Util/Util.hpp"

void DrawConnectionsComponent::Frame(float delta)
{
	bool attackedThisFrame = false;

	lines.clear();

	for (BaseEntity* entity : entMan->getNearbyEntities(owner, owner->maxViewDistance))
	{
		Line line;

		if (entity->team == Team::GoodGuys)
			line.setColor(sf::Color::Green);
		else if (entity->team == Team::BadGuys)
			line.setColor(sf::Color::Red);
		else if (entity->team == Team::Neutral)
			line.setColor(sf::Color::Yellow);

		line.setPoints(owner->getPosition(), entity->getPosition());

		if (!attackedThisFrame && entity->team == Team::BadGuys)
		{
			float dist = util::distance(owner->getPosition(), entity->getPosition());

			ComponentEntity* compEnt = dynamic_cast<ComponentEntity*>(owner);
			GroundMoveComponent* move = dynamic_cast<GroundMoveComponent*>(compEnt->hasComponent("GroundMove"));

			if (dist <= owner->maxAttackDistance)
			{
				// Don't attack if we're moving, and don't intercept entities while we're moving.
				if (move != nullptr && !move->getActive())
				{
					line.setColor(sf::Color::Black);
					entity->onDamage(1.0f, owner);
					attackedThisFrame = true;
				}
			}
			else // move into range
			{
				// TODO: only go as far as we need to to attack
				if (move != nullptr && !move->getActive())
					move->setMoveDestination(entity->getPosition());
			}
		}

		lines.push_back(line);
	}
}

void DrawConnectionsComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < lines.size(); i++)
		target.draw(lines[i].vertices, 4, sf::Quads);
}

Bob::Bob(const int entityID, EntityManager* entities) : entities(entities), ComponentEntity(entityID)
{
	logger::INFO("Bob entity created.");

	texture.loadFromFile("bobwars/resource/textures/bob.png"); // no point in logging failure, as SFML does this for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	team = Team::GoodGuys;
	maxHealth = 100;
	health = maxHealth;
	hitpoints = 1;
	type = "bob";

	actions.push_back({ "Create Comment Section", "Creates a Comment Section", sf::Keyboard::Key::C, "createcommentsection", static_cast<int>(EntityType::CommentSection) });

	addComponent(new GroundMoveComponent);
	addComponent(new DrawConnectionsComponent(entities));
}

Bob::~Bob()
{
}

BaseEntity::Status Bob::addTask(EntityType type)
{
	if (entities->entities.size() >= entities->maxEntsPerTeam)
		return Status::TooManyEntities;
	else if (tasks.size() >= maxTasks)
		return Status::TooManyTasks;

	switch (type)
	{
	case EntityType::CommentSection:
	{
		CommentSection* comment = new CommentSection(entities->getNextID(), entities);
		comment->setPosition(getPosition());
		entities->addEnt(comment);
		break;
	}
	default:
		break; 
	}

	logger::DEBUG("Added task to Bob.");
	return Status::Success;
}
