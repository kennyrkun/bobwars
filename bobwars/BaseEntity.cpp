#include "BaseEntity.hpp"

BaseEntity::BaseEntity()
{
	logger::SILENT("DEBUG", "Entity base class constructed.");

	m_texture.loadFromFile("resource\\textures\\bob.png");
	m_sprite.setTexture(m_texture);
}

BaseEntity::~BaseEntity()
{
	logger::SILENT("DEBUG", "Entity base class deconstructed.");
}
