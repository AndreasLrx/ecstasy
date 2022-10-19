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
        /// Add sentinel to allow use of firstUnset
        _alive.push(false);
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

        return Entity(firstDead, _generations[firstDead]);
    }

    Entities::Builder Entities::builder()
    {
        return Builder(*this, create(false));
    }

    Entity Entities::get(Entity::Index id) const
    {
        if (id > _generations.size())
            return Entity(id, 0);
        return Entity(id, _generations[id]);
    }

    bool Entities::kill(Entity entity)
    {
        Entity::Index id = entity.getIndex();

        if (id > _generations.size() || !_alive[id])
            return false;
        _alive[id] = false;
        return true;
    }

    bool Entities::isAlive(Entity entity) const
    {
        return entity.getGeneration() != 0 && entity.getIndex() < _generations.size() && _alive[entity.getIndex()];
    }
} // namespace ecstasy