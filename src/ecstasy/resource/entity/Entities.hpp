///
/// @file Entities.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_RESOURCE_ENTITY_ENTITIES_HPP_
#define ECSTASY_RESOURCE_ENTITY_ENTITIES_HPP_

#include <vector>

#include "Entity.hpp"
#include "ecstasy/resource/Resource.hpp"
#include "util/BitSet.hpp"

namespace ecstasy
{
    ///
    /// @brief Entities Resource.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-18)
    ///
    class Entities : public Resource {
      public:
        ///
        /// @brief Construct a new Entities resource without any entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        Entities();

        ///
        /// @brief Create a new entity, alive or not.
        ///
        /// @param[in] alive Whether the entity must be created alive or not.
        ///
        /// @return Entity Newly created entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        Entity create(bool alive = true);

        ///
        /// @brief Retrieve an entity from its identifier.
        ///
        /// @param[in] id Index of the entity.
        ///
        /// @return Entity
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        Entity get(Entity::Index id) const;

        ///
        /// @brief Killing the entity when called.
        ///
        /// @note Entity is considered kill if it was alive.
        ///
        /// @param[in] entity Entity to kill.
        ///
        /// @return bool Whether the entity was killed or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        bool kill(Entity entity);

        ///
        /// @brief Tests if an entity is alive.
        ///
        /// @param[in] entity entity to test.
        ///
        /// @return bool If the entity is alive.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        bool isAlive(Entity entity) const;

      private:
        std::vector<Entity::Generation> _generations;
        util::BitSet _alive;
    };
} // namespace ecstasy

#endif /* !ECSTASY_RESOURCE_ENTITY_ENTITIES_HPP_ */
