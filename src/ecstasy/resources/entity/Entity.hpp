///
/// @file Entity.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-18
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_RESOURCE_ENTITY_ENTITY_HPP_
#define ECSTASY_RESOURCE_ENTITY_ENTITY_HPP_

#include <compare>
#include <stddef.h>
#include <stdexcept>

#include "ecstasy/storages/IStorage.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"

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
        /// @return Index entity index (identifier)
        ///
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
        /// @note A generation greater than 0 doesn't mean the entity is always alive.
        ///
        /// @return Generation The generation of this entity.
        ///
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

        ///
        /// @brief Compare two entities using only their identifier.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        constexpr bool operator==(Entity const &other) const
        {
            return this->_index == other._index;
        }

        ///
        /// @brief Add a component to the entity.
        ///
        /// @tparam C Component type.
        /// @tparam Args Type of the arguments to forward to the component constructor.
        ///
        /// @param[in] storage Storage for type @b C.
        /// @param[in] args Arguments to forward to the component constructor.
        ///
        /// @return C& Reference to the newly created component.
        ///
        /// @throw std::logic_error If the component was already present.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <IsStorage S, typename... Args>
        typename S::Component &add(S &storage, Args &&...args)
        {
            if (storage.contains(_index))
                throw std::logic_error(std::string("Trying to add twice the component ")
                    + typeid(typename S::Component).name() + " on the same entity.");
            return storage.emplace(_index, std::forward<Args>(args)...);
        }

        ///
        /// @brief If the entity already has an instance of component @b C, returns it. If it doesn't, create a new
        /// instance of @b C for this entity and return it.
        ///
        /// @note You should use @ref add to create new components with constructor parameters and @ref get to retrieve
        /// them.
        ///
        /// @tparam C Type of the component to retrieve.
        ///
        /// @param[in] storage Storage for type @b C.
        ///
        /// @return C& Reference to the entity instance of @b C associated to the entity. May be a new component.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <IsStorage S>
        typename S::Component &operator[](S &storage)
        {
            if (!storage.contains(_index))
                return storage.emplace(_index);
            return storage[_index];
        }

        ///
        /// @brief Try to fetch the instance of component @b C associated to the current entity.
        ///
        /// @tparam C Type of the component to retrieve.
        ///
        /// @param[in] storage Storage for type @b C.
        ///
        /// @return const C& Reference to the entity instance of @b C associated to the entity.
        ///
        /// @throw std::out_of_range If no associated instance found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <IsStorage S>
        const typename S::Component &operator[](S &storage) const
        {
            return storage[_index];
        }

        ///
        /// @brief Try to fetch the instance of component @b C associated to the current entity.
        ///
        /// @tparam C Type of the component to retrieve.
        ///
        /// @param[in] storage Storage for type @b C.
        ///
        /// @return const C& Const reference to the entity instance of @b C associated to the entity.
        ///
        /// @throw std::out_of_range If no associated instance found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <IsStorage S>
        const typename S::Component &get(S &storage) const
        {
            return storage[_index];
        }

        ///
        /// @brief Try to fetch the instance of component @b C associated to the current entity.
        ///
        /// @tparam C Type of the component to retrieve.
        ///
        /// @param[in] storage Storage for type @b C.
        ///
        /// @return C& Reference to the entity instance of @b C associated to the entity.
        ///
        /// @throw std::out_of_range If no associated instance found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <IsStorage S>
        typename S::Component &get(S &storage)
        {
            return storage[_index];
        }

        ///
        /// @brief Test if the entity has an associated component in the storage @b S.
        ///
        /// @tparam S Storage of a component.
        ///
        /// @param[in] storage Storage for the component type searched.
        ///
        /// @return bool True if the entity has an associated entry in the storage.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <IsStorage S>
        bool has(S &storage) const
        {
            return storage.contains(_index);
        }

      protected:
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
