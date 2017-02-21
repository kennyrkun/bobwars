#include "BaseEntity.hpp"

#include <ENGINE\logger.hpp>

//-------------------------------------
/// Defines the BaseEntity entry point.
//-------------------------------------
BaseEntity::BaseEntity()
{
	logger::DEBUG("new entity created");
	m_health = 100;
}

//-------------------------------------
/// Defines BaseEntity destructor.
//-------------------------------------
BaseEntity::~BaseEntity()
{
	logger::DEBUG("entity removed");
}
