#ifndef BUILDING_ENTITY_HPP
#define BUILDING_ENTITY_HPP

#include "BaseEntity.hpp"

class BuildingEntity : public BaseEntity
{
public:
	BuildingEntity(const int entityID, AppEngine* app = nullptr) : BaseEntity(entityID, true, true, false, false, false, EntityType::BuildingEntity, app) {}

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

#endif // !BUILDING_ENTITY_HPP

