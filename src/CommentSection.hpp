#ifndef COMMENT_SECTION_HPP
#define COMMENT_SECTION_HPP

#include "Building.hpp"
#include "EntityManager.hpp"

#include "Util/Graphics/ProgressBar.hpp"

#include <vector>

class CommentSection : public Building
{
public:
	CommentSection(const int entityID, EntityManager* manager);
	~CommentSection();

	void setPosition(const sf::Vector2f& position);

	BaseEntity::Status addTask(EntityType type) override;

	void Frame(const float delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	EntityManager* entities;

	ProgressBar progressBar;
};

#endif // !COMMENT_SECTION_HPP
