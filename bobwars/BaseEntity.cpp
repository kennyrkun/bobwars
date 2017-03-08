#include "BaseEntity.hpp"

BaseEntity::BaseEntity()
{
	logger::DEBUG("entity base class constructed.");
	m_health = 100;
}

BaseEntity::~BaseEntity()
{
	logger::DEBUG("entity base class deconstructed.");
}
