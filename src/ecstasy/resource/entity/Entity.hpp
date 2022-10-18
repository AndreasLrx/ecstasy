///
/// @file Entity.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-18
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_RESOURCE_ENTITY_ENTITY_HPP_
#define ECSTASY_RESOURCE_ENTITY_ENTITY_HPP_

#include <compare>
#include <stddef.h>

namespace ecstasy
{
    class Entities;

    ///
    /// @brief Encapsulate an index to an entity.
    ///
    /// @note The entity may not exist (ie an id doesn't mean a living entity)
    /// @note This class is trivially copiable because it doesn't store the entity components.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-18)
    ///
    class Entity {
      public:
        /// @brief The entity identifier type.
        using Index = size_t;
        /// @brief The entity generation type.
        using Generation = size_t;

        ///
        /// @brief Get the entity identifier,
        ///
        /// @return constexpr Index entity index (identifier)
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        constexpr Index getIndex() const
        {
            return _index;
        }

        ///
        /// @brief Get the Generation of the entity. Alive entities always have a generation number strictly positive.
        /// If two entities have the same id, it mean the one with the lowest generation is outdated and no longer
        /// exists.
        ///
        /// @return constexpr Generation The generation of this entity.
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        constexpr Generation getGeneration() const
        {
            return _generation;
        }

        ///
        /// @brief Compare two entities using only their identifier. Used to sort entities.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        constexpr auto operator<=>(Entity const &other) const
        {
            return this->_index <=> other._index;
        }

      private:
        Index _index;
        Generation _generation;

        ///
        /// @brief The entity structure may only be constructed by builders.
        ///
        /// @param[in] index The entity identifier.
        /// @param[in] generation The entity generation.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        constexpr Entity(Index index, Generation generation) : _index(index), _generation(generation){};

        /// The 'Entities' Resource is the only class able to create entities.
        friend class ::ecstasy::Entities;
    };
} // namespace ecstasy

#endif /* !ECSTASY_RESOURCE_ENTITY_ENTITY_HPP_ */
