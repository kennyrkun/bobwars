#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <ENGINE\Engine.hpp>

class BaseEntity
{
public:
	sf::CircleShape move_dest;
	sf::Sprite m_sprite;
	int m_health;
	int m_id;
	int m_team;
	bool m_moving;

	BaseEntity();
	~BaseEntity();

	void moveTo(const sf::Vector2f &pos);
	void Update();

private:
	sf::Vector2f movePos;
	sf::Texture m_texture;
};

#endif // BASE_ENTITY_HPP
