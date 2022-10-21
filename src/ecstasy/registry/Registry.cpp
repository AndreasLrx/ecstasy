///
/// @file Registry.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "Registry.hpp"
#include "ecstasy/resource/entity/Entities.hpp"

namespace ecstasy
{
    Registry::EntityBuilder::EntityBuilder(Registry &registry)
        : _registry(registry), _builder(registry.getResource<Entities>().builder())
    {
    }

    Entity Registry::EntityBuilder::build()
    {
        return _builder.build();
    }

    Registry::Registry(bool addEntities)
    {
        if (addEntities)
            addResource<Entities>();
    }

    Registry::EntityBuilder Registry::entityBuilder()
    {
        return EntityBuilder(*this);
    }

    Entity Registry::getEntity(Entity::Index index) noexcept
    {
        return getResource<Entities>().get(index);
    }

    bool Registry::erase(Entity::Index index)
    {
        bool erased = getResource<Entities>().erase(index);

        if (!erased)
            return false;
        for (auto &storage : _storages.getInner())
            storage.second->erase(index);
        return true;
    }

    void Registry::runSystems()
    {
        for (auto &[type, system] : _systems.getInner())
            system->run(*this);
    }
} // namespace ecstasy