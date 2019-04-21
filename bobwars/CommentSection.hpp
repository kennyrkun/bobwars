#ifndef COMMENT_SECTION_HPP
#define COMMENT_SECTION_HPP

#include "BaseEntity.hpp"

class CommentSection : public BaseEntity
{
public:
	CommentSection(const int entityID);
	~CommentSection();

	sf::Vector2f garrisonPoint;
};

#endif // !COMMENT_SECTION_HPP
