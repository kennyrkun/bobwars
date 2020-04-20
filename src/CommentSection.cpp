#include "CommentSection.hpp"

#include "Util/Logger.hpp"

CommentSection::CommentSection(const int entityID, EntityManager* manager, const int maxTasks) : entities(manager), maxTasks(maxTasks), Building(entityID)
{
	logger::INFO("CommentSection entity created.");

	texture.loadFromFile("bobwars/resource/textures/commentsection.png"); // no point in logging failure, as SFML does this for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	team = Team::GoodGuys;
	maxHealth = 200;
	health = maxHealth;
	type = "commentsection";

	actions.push_back({ "Create Bob", "Creates a Bob", sf::Keyboard::Key::B, "bob.png" });
	actions.push_back({ "Create Boomer", "Creates a Boomer", sf::Keyboard::Key::E, "boomer.png" });
}

CommentSection::~CommentSection()
{
}

void CommentSection::Frame(const float delta)
{
	for (Task task : tasks)
	{
		if ((time(0) - task.startTime) > task.duration)
		{
			logger::INFO("task should be finished");

			switch (task.type)
			{
			case EntityType::Bob:
			{
				Bob* bob = entities->newBob();
				bob->setPosition(entities->selectedEnts[0]->getPosition());

				if (hasGarrisonPoint)
				{
					GroundMoveComponent* move = dynamic_cast<GroundMoveComponent*>(bob->hasComponent("GroundMove"));
					move->setMoveDestination(getGarrisonPoint());
					//bob->getComponent<GroundMoveComponent>("GroundMove")->setMoveDestination(getGarrisonPoint());
				}

				task.finished = true;
				break;
			}
			case EntityType::Boomer:
			{
				break;
			}
			default:
				break;
			}
		}
	}
}

CommentSection::Status CommentSection::addTask(EntityType type)
{
	if (entities->entities.size() >= entities->maxEntsPerTeam)
		return Status::TooManyEntities;

	switch (type)
	{
	case EntityType::Bob:
	{
		tasks.push_back({ type, time(0), 10.0f });
		break;
	}
	case EntityType::Boomer:
	{
		tasks.push_back({ type, time(0), 25.0f });
		break;
	}
	default:
		break; 
	}

	logger::DEBUG("Added task to CommentSection.");
	return Status::Success;
}