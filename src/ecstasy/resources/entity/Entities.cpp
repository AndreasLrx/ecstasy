///
/// @file Entities.cpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#include "Entities.hpp"

namespace ecstasy
{
    Entities::Builder::Builder(Entities &parent, Entity entity) : _parent(parent), _entity(entity), _built(false)
    {
    }

    void Entities::Builder::assertNotBuilt() const
    {
        if (_built)
            throw std::logic_error(
                "Try to change entity using an Entities::Builder already consumed (build() has been called)");
    }

    Entity Entities::Builder::build()
    {
        assertNotBuilt();
        _built = true;
        _parent._alive[_entity.getIndex()] = true;
        return _entity;
    }

    Entities::Entities()
    {
        /// Add sentinel to allow use of firstUnset/firstSet
        _alive.push(false);
        _killed.push(true);
    }

    Entity Entities::create(bool alive)
    {
        size_t firstDead = _alive.firstUnset();

        /// No index available, need to add one at the end
        if (firstDead == _generations.size()) {
            _generations.push_back(1);
            _alive[firstDead] = alive;
            /// Always add a sentinel bit
            _alive.push(false);

            return Entity(firstDead, 1);
        }
        /// One index available, use it
        _generations[firstDead]++;
        _alive[firstDead] = alive;
        /// Remove deletion marker
        if (firstDead < _killed.size() - 1)
            _killed[firstDead] = false;
        return Entity(firstDead, _generations[firstDead]);
    }

    Entities::Builder Entities::builder()
    {
        return Builder(*this, create(false));
    }

    Entity Entities::get(Entity::Index id) const noexcept
    {
        if (id >= _generations.size())
            return Entity(id, 0);
        return Entity(id, _generations[id]);
    }

    bool Entities::erase(Entity entity)
    {
        Entity::Index id = entity.getIndex();

        if (id >= _generations.size() || entity.getGeneration() != _generations[id] || !_alive[id])
            return false;
        _alive[id] = false;
        return true;
    }

    size_t Entities::erase(std::span<Entity> entities)
    {
        size_t res = 0;

        for (Entity entity : entities)
            res += erase(entity);
        return res;
    }

    bool Entities::kill(Entity entity)
    {
        Entity::Index id = entity.getIndex();

        if (id >= _generations.size() || !_alive[id] || entity.getGeneration() != _generations[id])
            return false;
        if (id > _killed.size() - 1)
            _killed.resizeSentinel(id + 1, true);
        _killed[id] = true;
        return true;
    }

    bool Entities::isAlive(Entity entity) const
    {
        return entity.getGeneration() != 0 && entity.getIndex() < _generations.size() && _alive[entity.getIndex()];
    }

    Entity Entities::getQueryData(Entity::Index index) const
    {
        return get(index);
    }

    std::vector<Entity> Entities::maintain()
    {
        std::vector<Entity> deleted;
        size_t firstKilled = _killed.firstSet();

        while (firstKilled != _killed.size() - 1) {
            _alive[firstKilled] = false;
            _killed[firstKilled] = false;
            /// Cannot emplace_back because constructor is private in the vector context :/
            deleted.push_back(Entity(firstKilled, _generations[firstKilled]));
            firstKilled = _killed.firstSet(firstKilled);
        }
        return deleted;
    }

} // namespace ecstasy