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
        : _registry(registry), _builder(registry.getResource<Entities>().get().builder())
    {
    }

    Entity Registry::EntityBuilder::build()
    {
        return _builder.build();
    }

    Registry::Registry(bool addEntities) : _pipeline(*this)
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
        return getResource<const Entities>().get().get(index);
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
        bool erased = getResource<Entities>().get().erase(entity);

        if (!erased)
            return false;
        eraseEntityComponents(entity);
        return true;
    }

    size_t Registry::eraseEntities(std::span<Entity> entities)
    {
        size_t erased = getResource<Entities>().get().erase(entities);

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

    void Registry::runSystem(const std::type_index &systemId)
    {
        ISystem &system = _systems.get(systemId);

        if (system.getTimer().trigger())
            system.run(*this);
    }

    void Registry::runSystems()
    {
        _pipeline.run();
    }

    void Registry::runSystemsPhase(Pipeline::PhaseId phase)
    {
        _pipeline.run(phase);
    }

    void Registry::clear()
    {
        _resources.clear();
        _storages.clear();
        _systems.clear();
        addResource<Entities>();
    }

} // namespace ecstasy
