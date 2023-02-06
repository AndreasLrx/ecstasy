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
#include "ecstasy/resources/entity/Entities.hpp"

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

    const Entities &Registry::getEntities() const
    {
        return getResource<Entities>();
    }

    Entities &Registry::getEntities()
    {
        return getResource<Entities>();
    }

    Entity Registry::getEntity(Entity::Index index) noexcept
    {
        return getResource<Entities>().get(index);
    }

    bool Registry::eraseEntity(Entity entity)
    {
        bool erased = getResource<Entities>().erase(entity);

        if (!erased)
            return false;
        eraseEntityComponents(entity);
        return true;
    }

    size_t Registry::eraseEntities(std::span<Entity> entities)
    {
        size_t erased = getResource<Entities>().erase(entities);

        if (!erased)
            return 0;
        eraseEntitiesComponents(entities);
        return erased;
    }

    void Registry::eraseEntityComponents(Entity entity)
    {
        for (auto &storage : _storages.getInner())
            storage.second->erase(std::span{&entity, 1});
    }

    void Registry::eraseEntitiesComponents(std::span<Entity> entities)
    {
        for (auto &storage : _storages.getInner())
            storage.second->erase(entities);
    }

    void Registry::runSystems()
    {
        for (auto &[type, system] : _systems.getInner())
            system->run(*this);
    }

    void Registry::runSystems(size_t group, size_t mask)
    {
        auto &systems = _systems.getInner();

        auto it = std::find_if(systems.begin(), systems.end(), [&group, &mask](auto &entry) {
            return (entry.first.second & mask) == group;
        });

        while (it != systems.end()) {
            it->second->run(*this);
            ++it;
            if ((it->first.second & mask) != group)
                break;
        }
    }

    void Registry::clear()
    {
        _resources.clear();
        _storages.clear();
        _systems.clear();
        addResource<Entities>();
    }

} // namespace ecstasy
