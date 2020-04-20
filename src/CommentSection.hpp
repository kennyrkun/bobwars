#ifndef COMMENT_SECTION_HPP
#define COMMENT_SECTION_HPP

#include "Building.hpp"
#include "EntityManager.hpp"

#include <vector>

class CommentSection : public Building
{
public:
	CommentSection(const int entityID, EntityManager* manager, const int maxTasks = 15);
	~CommentSection();

	struct Task
	{
		EntityType type;
		time_t startTime = 0;
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

	void Frame(const float delta) override;

	Status addTask(EntityType type);

	std::vector<Task> tasks;

private:
	EntityManager* entities;
};

#endif // !COMMENT_SECTION_HPP
