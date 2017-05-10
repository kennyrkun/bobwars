#include "BaseEntity.hpp"

BaseEntity::BaseEntity()
{
	logger::SILENT("DEBUG", "entity base class constructed.");
}

BaseEntity::~BaseEntity()
{
	logger::SILENT("DEBUG", "entity base class deconstructed.");
}
