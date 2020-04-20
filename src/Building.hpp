#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "BaseEntity.hpp"

#include <SFML/Graphics/CircleShape.hpp>

class Building : public BaseEntity
{
public:
	Building(const int entityID, AppEngine* app = nullptr) : BaseEntity(entityID, true, true, false, false, false, EntityType::BuildingEntity, app)
	{
		garrisonPoint.setRadius(2);
		garrisonPoint.setOrigin(sf::Vector2f(1, 1));
	}

	inline void setGarrisonPoint(const sf::Vector2f& point)
	{
		garrisonPoint.setPosition(point);
	}

	inline const sf::Vector2f& getGarrisonPoint() const
	{
		return garrisonPoint.getPosition();
	}

	bool hasGarrisonPoint = false;

	inline void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		BaseEntity::draw(target, states);

		if (isSelected)
			target.draw(garrisonPoint);
	}

private:
	sf::CircleShape garrisonPoint;
};

#endif // !BUILDING_HPP

