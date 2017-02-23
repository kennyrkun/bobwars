#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <ENGINE\engine_main.hpp>

#include <string>

class BaseEntity
{
public:
	int m_health;
	int m_id;
	sf::Sprite m_sprite;
	std::string m_name;
	bool m_active;

	BaseEntity();
	~BaseEntity();
};

#endif
