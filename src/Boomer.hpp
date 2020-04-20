#ifndef BOOMER_HPP
#define BOOMER_HPP

#include "ComponentEntity.hpp"
#include "EntityManager.hpp"

class Boomer : public ComponentEntity
{
public:
    Boomer(int entityID, EntityManager* app);

    void Frame(const float delta) override;

private:
    EntityManager* entities;
};

#endif // !BOOMER_HPP