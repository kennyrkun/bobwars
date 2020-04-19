#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "BaseEntity.hpp"

class Building : public BaseEntity
{
public:
	Building(const int entityID, AppEngine* app = nullptr) : BaseEntity(entityID, true, true, false, false, false, EntityType::BuildingEntity, app) {}

	inline void setGarrisonPoint(const sf::Vector2f& point)
	{
		garrisonPoint = point;
	}

	inline const sf::Vector2f& getGarrisonPoint() const
	{
		return garrisonPoint;
	}

	bool hasGarrisonPoint;

private:
	sf::Vector2f garrisonPoint;
};

#endif // !BUILDING_HPP

