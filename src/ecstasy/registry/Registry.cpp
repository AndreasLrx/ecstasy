///
/// @file Registry.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#include "Registry.hpp"
#include "ecstasy/resources/entity/Entities.hpp"

namespace ecstasy
{
    Registry::EntityBuilder::EntityBuilder(Registry &registry) noexcept
        : _registry(registry), _builder(registry.getResource<Entities>()->builder())
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

    Registry::EntityBuilder Registry::entityBuilder() noexcept
    {
        return EntityBuilder(*this);
    }

    Entity Registry::getEntity(Entity::Index index)
    {
        return getResource<const Entities>()->get(index);
    }

    std::vector<std::reference_wrapper<IStorage>> Registry::getEntityStorages(Entity entity)
    {
        std::vector<std::reference_wrapper<IStorage>> storages;
        for (auto &storage : _storages.getInner())
            if (storage.second->contains(entity.getIndex()))
                storages.push_back(*storage.second);
        return storages;
    }

    bool Registry::eraseEntity(Entity entity)
    {
        bool erased = getResource<Entities>()->erase(entity);

        if (!erased)
            return false;
        eraseEntityComponents(entity);
        return true;
    }

    size_t Registry::eraseEntities(std::span<Entity> entities)
    {
        size_t erased = getResource<Entities>()->erase(entities);

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
        auto it = systems.begin();

        // Run all systems with the same group and mask
        while (it != systems.end()) {
            if ((it->first.second & mask) == group)
                it->second->run(*this);
            ++it;
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
