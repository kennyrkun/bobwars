#ifndef COMMENT_SECTION_HPP
#define COMMENT_SECTION_HPP

#include "Building.hpp"
#include "EntityManager.hpp"

#include "Util/Graphics/ProgressBar.hpp"

#include <vector>

class CommentSection : public Building
{
public:
	CommentSection(const int entityID, EntityManager* manager, const int maxTasks = 15);
	~CommentSection();

	struct Task
	{
		EntityType type;
		float progress;
		float duration; // Seconds
		bool finished = false;
	};

	enum class Status
	{
		NotEnoughResource,
		TooManyEntities,
		TooManyTasks,

		Success
	} fail;

	const int maxTasks = 15;

	std::vector<Task> tasks;

	Status addTask(EntityType type);

	void setPosition(const sf::Vector2f& position);

	void Frame(const float delta) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	EntityManager* entities;

	ProgressBar progressBar;
};

#endif // !COMMENT_SECTION_HPP
