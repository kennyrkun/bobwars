#include "CommentSection.hpp"

#include "Bob.hpp"
#include "Boomer.hpp"

#include "Util/Logger.hpp"

CommentSection::CommentSection(const int entityID, EntityManager* manager) : entities(manager), Building(entityID)
{
	logger::INFO("CommentSection entity created.");

	texture.loadFromFile("bobwars/resource/textures/commentsection.png"); // no point in logging failure, as SFML does this for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	team = Team::GoodGuys;
	maxHealth = 200;
	health = maxHealth;
	type = "commentsection";

	actions.push_back({ "Create Bob", "Creates a Bob", sf::Keyboard::Key::B, "createbob", static_cast<int>(EntityType::Bob) });
	actions.push_back({ "Create Boomer", "Creates a Boomer", sf::Keyboard::Key::E, "createboomer", static_cast<int>(EntityType::Boomer) });

	progressBar.setWidth(sprite.getGlobalBounds().width);
	progressBar.setHeight(10);

	sf::Vector2f barPos = getPosition();
	barPos.x -= sprite.getLocalBounds().width / 2;
	barPos.y += sprite.getLocalBounds().height / 2;
	progressBar.setPosition(barPos);
}

CommentSection::~CommentSection()
{
}

BaseEntity::Status CommentSection::addTask(EntityType type)
{
	if (entities->entities.size() >= entities->maxEntsPerTeam)
		return Status::TooManyEntities;
	else if (tasks.size() >= maxTasks)
		return Status::TooManyTasks;

	switch (type)
	{
	case EntityType::Bob:
	{
		tasks.push_back({ type, 1.0f });
		break;
	}
	case EntityType::Boomer:
	{
		tasks.push_back({ type, 10.0f });
		break;
	}
	default:
		break; 
	}

	logger::DEBUG("Added task to CommentSection.");
	return Status::Success;
}

void CommentSection::setPosition(const sf::Vector2f& position)
{
	Building::setPosition(position);
	
	sf::Vector2f barPos = getPosition();
	barPos.x -= sprite.getLocalBounds().width / 2;
	barPos.y += sprite.getLocalBounds().height / 2;
	progressBar.setPosition(barPos);
}

void CommentSection::Frame(const float delta)
{
	if (tasks.empty())
		return;

	Task& task = tasks.front();

	if (task.progress > 100)
	{
		logger::INFO("Task should be complete");

		switch (task.type)
		{
		case EntityType::Bob:
		{
			Bob* bob = new Bob(entities->getNextID(), entities);
			bob->setPosition(getPosition());
			entities->addEnt(bob);

			if (hasGarrisonPoint)
			{
				logger::INFO("Moving Bob");
				GroundMoveComponent* move = dynamic_cast<GroundMoveComponent*>(bob->hasComponent("GroundMove"));
				move->setMoveDestination(getGarrisonPoint());
			}

			task.finished = true;
			break;
		}
		case EntityType::Boomer:
		{
			Boomer* boomer = new Boomer(entities->getNextID(), entities);
			boomer->setPosition(getPosition());
			entities->addEnt(boomer);

			if (hasGarrisonPoint)
			{
				logger::INFO("Moving Boomer");
				GroundMoveComponent* move = dynamic_cast<GroundMoveComponent*>(boomer->hasComponent("GroundMove"));
				move->setMoveDestination(getGarrisonPoint());
			}

			task.finished = true;
			break;
		}
		default:
			break;
		}

		tasks.erase(tasks.begin());
	}
	else
	{
		task.progress += 1;
		progressBar.setValue(task.progress, 100);
	}

	Building::Frame(delta);
}

void CommentSection::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Building::draw(target, states);

	if (!tasks.empty())
		target.draw(progressBar, states);
}