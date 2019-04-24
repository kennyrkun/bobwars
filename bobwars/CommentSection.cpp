#include "CommentSection.hpp"

#include "Util/Logger.hpp"

CommentSection::CommentSection(const int entityID) : BaseEntity(entityID, true, true, false, false, false, EntityType::CommentSection)
{
	logger::INFO("CommentSection entity created.");

	texture.loadFromFile("bobwars/resource/textures/commentsection.png"); // no point in logging failure, as SFML does this for us (I wish it didn't!)
	sprite.setTexture(texture, true);
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2));

	moveDest.setRadius(3);
	moveDest.setPointCount(8);
	moveDest.setFillColor(sf::Color::Yellow);
	moveDest.setOrigin(sf::Vector2f(moveDest.getLocalBounds().width / 2, moveDest.getLocalBounds().height / 2));

	team = 1;
	health = 200;
	type = "commentsection";
}

CommentSection::~CommentSection()
{
}
