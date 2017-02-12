#pragma once
#include <ENGINE\engine_main.hpp>

#include <string>

class BaseEntity
{
public:
	int m_health;
	sf::Sprite m_sprite;
	std::string m_name;

	BaseEntity();
	~BaseEntity();
};
