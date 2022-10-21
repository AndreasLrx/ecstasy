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

    bool Registry::erase(Entity entity)
    {
        bool erased = getResource<Entities>().erase(entity);

        if (!erased)
            return false;
        for (auto &storage : _storages.getInner())
            storage.second->erase(entity.getIndex());
        return true;
    }

    void Registry::runSystems()
    {
        for (auto &[type, system] : _systems.getInner())
            system->run(*this);
    }
} // namespace ecstasy