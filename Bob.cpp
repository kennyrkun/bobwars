#include "Bob.hpp"

#include "Util/Logger.hpp"

void GroundMoveComponent::setDestination(const sf::Vector2f destination)
{
	this->destination = destination;
	moveInProgress = true;
}

void GroundMoveComponent::cancelMovement()
{
	moveInProgress = false;
}

void GroundMoveComponent::Frame(float delta)
{
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

	team = 1;
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
}

Bob::~Bob()
{
}
