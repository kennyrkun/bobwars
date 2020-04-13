#include "CommentSection.hpp"

#include "Util/Logger.hpp"

CommentSection::CommentSection(const int entityID) : BuildingEntity(entityID)
{
	logger::INFO("CommentSection entity created.");

	texture.loadFromFile("bobwars/resource/textures/commentsection.png"); // no point in logging failure, as SFML does this for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	team = Team::GoodGuys;
	maxHealth = 200;
	health = maxHealth;
	type = "commentsection";
}

CommentSection::~CommentSection()
{
}

