#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <ENGINE\Engine.hpp>

class BaseEntity
{
public:
	int m_health;
	int m_id;
	int m_team;
	sf::Sprite m_sprite;
	//bool m_active;

	BaseEntity();
	~BaseEntity();
};

#endif // BASE_ENTITY_HPP
