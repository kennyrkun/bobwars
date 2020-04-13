#include "Bob.hpp"

#include "Util/Logger.hpp"
#include "EntityManager.hpp"

void GroundMoveComponent::setDestination(const sf::Vector2f destination)
{
	this->destination = destination;
	moveInProgress = true;
}

void GroundMoveComponent::cancelMovement()
{
	moveInProgress = false;
}

void DrawConnectionsComponent::Frame(float delta)
{
	lines.clear();

	for (BaseEntity* entity : entMan->getNearbyEntities(owner, 100, 100, nullptr))
	{
		Line line;

		if (entity->team == Team::GoodGuys)
			line.setColor(sf::Color::Green);
		else if (entity->team == Team::BadGuys)
			line.setColor(sf::Color::Red);
		else if (entity->team == Team::Neutral)
			line.setColor(sf::Color::Red);
		else
			line.setColor(sf::Color::Black);

		line.setPoints(owner->getPosition(), entity->getPosition());
		lines.push_back(line);

		if (entity->team != owner->team)
			entity->onDamage(1.0f, owner);
	}
}

void DrawConnectionsComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < lines.size(); i++)
		target.draw(lines[i].vertices, 4, sf::Quads);
}

void GroundMoveComponent::Frame(float delta)
{
//	linear interpolation: c * t / d + b

	if (moveInProgress)
	{
		int sX = static_cast<int>(owner->sprite.getPosition().x);
		int sY = static_cast<int>(owner->sprite.getPosition().y);
		int gX = static_cast<int>(destination.x);
		int gY = static_cast<int>(destination.y);

		if (sX > gX)
			owner->sprite.move(-1.f, 0);
		else if (sX < gX)
			owner->sprite.move(1.f, 0);

		if (sY > gY)
			owner->sprite.move(0, -1.f);
		else if (sY < gY)
			owner->sprite.move(0, 1.f);

		if (sX == gX && sY == gY)
		{
			logger::INFO("Done moving! (" + std::to_string(owner->entityID) + ")");
			moveInProgress = false;
		}
	}
}

Bob::Bob(const int entityID) : ComponentEntity(entityID)
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

	EntityAction createCommentSectionAction;
	createCommentSectionAction.name = "Create CommentSection";
	createCommentSectionAction.description = "Creates a CommentSection";
	createCommentSectionAction.hotkey = sf::Keyboard::Key::C;
	createCommentSectionAction.buttonTexture = "./bobwars/resources/textures/spritebuttons/commentsection.png";

	actions.push_back(createCommentSectionAction);

	addComponent(new GroundMoveComponent);
	// TODO: this component can only be added with an instance of EntityManager
//	addComponent(new DrawConnectionsComponent);
}

Bob::~Bob()
{
}
