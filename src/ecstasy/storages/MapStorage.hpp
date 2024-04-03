///
/// @file MapStorage.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_STORAGE_MAPSTORAGE_HPP_
#define ECSTASY_STORAGE_MAPSTORAGE_HPP_

#include <unordered_map>

#include "IStorage.hpp"
#include "ecstasy/resources/entity/Entity.hpp"
#include "util/BitSet.hpp"

namespace ecstasy
{
    ///
    /// @brief Associative Map to store entity components
    ///
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-19)
    ///
    template <typename C>
    class MapStorage : public IStorage {
      public:
        /// @brief IsStorage constraint
        using Component = C;

        /// @brief @ref ecstasy::query::QueryableObject constraint.
        using QueryData = C &;
        /// @brief @ref ecstasy::query::ConstQueryableObject constraint.
        using ConstQueryData = const C &;

        ///
        /// @brief Construct a new Map Storage for a given Component type.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        MapStorage() = default;

        ///
        /// @brief Copy constructor is deleted.
        ///
        /// @param[in] other Storage to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        MapStorage(const MapStorage &other) = delete;

        ///
        /// @brief Emplace a new @b Component instance for a given entity.
        ///
        /// @note No check is done to see if the entity already has the component.
        ///
        /// @tparam Args Type of the arguments to forward to the component constructor.
        ///
        /// @param[in] index Entity index.
        /// @param[in] args Args to forward to the component constructor.
        ///
        /// @return Component& Newly created component.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <typename... Args>
        Component &emplace(Entity::Index index, Args &&...args)
        {
            _mask.resize(std::max(_mask.size(), index + 1));
            _mask[index] = true;
            return _components.emplace(std::make_pair(index, Component(std::forward<Args>(args)...))).first->second;
        }

        ///
        /// @brief Erase the @b Component instance associated to the given entity.
        ///
        /// @note Does nothing if the index doesn't match with any component (ie if the entity doesn't have a component
        /// @b Component)
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        bool erase(Entity::Index index)
        {
            auto it = _components.find(index);

            if (it != _components.end()) {
                _components.erase(index);
                _mask[index] = false;
                return true;
            }
            return false;
        }

        ///
        /// @brief Erase multiple @b Component instances associated to the given @p entities.
        ///
        /// @note Does nothing for entity without attached component (ie if the entity doesn't have a component
        /// @b Component)
        ///
        /// @param[in] entities target entities.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        void erase(std::span<Entity> entities) override final
        {
            for (Entity entity : entities)
                erase(entity.getIndex());
        }

        ///
        /// @brief Retrieve the @b Component instance associated to the given entity.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return const Component& Const reference to the associated component.
        ///
        /// @throw std::out_of_range If the entity doesn't have the component.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        const Component &operator[](Entity::Index index) const
        {
            return _components.at(index);
        }

        ///
        /// @brief Retrieve the @b Component instance associated to the given entity.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return Component& Reference to the associated component.
        ///
        /// @throw std::out_of_range If the entity doesn't have the component.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        Component &operator[](Entity::Index index)
        {
            return _components.at(index);
        }

        ///
        /// @brief Retrieve the @b Component instance associated to the given entity.
        ///
        /// @note @ref ecstasy::query::QueryableObject constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return Component& Reference to the associated component.
        ///
        /// @throw std::out_of_range If the entity doesn't have the component.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        Component &getQueryData(Entity::Index index)
        {
            return _components.at(index);
        }

        ///
        /// @brief Retrieve the const @b Component instance associated to the given entity.
        ///
        /// @note @ref ecstasy::query::ConstQueryableObject constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return const Component& Const reference to the associated component.
        ///
        /// @throw std::out_of_range If the entity doesn't have the component.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        const Component &getQueryData(Entity::Index index) const
        {
            return _components.at(index);
        }

        ///
        /// @brief Test if the entity index match a @b Component instance.
        ///
        /// @param[in] index
        /// @return bool
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        bool contains(Entity::Index index) const
        {
            return _components.find(index) != _components.end();
        }

        ///
        /// @brief Get the number of @b Component instances.
        ///
        /// @return size_t number of @b Component instances.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        size_t size() const noexcept
        {
            return _components.size();
        }

        ///
        /// @brief Get the Component Mask.
        ///
        /// @note Each bit set to true mean the entity at the bit index has a component @b C.
        /// @note @ref ecstasy::query::QueryableObject constraint.
        /// @warning The mask might be smaller than the entity count.
        ///
        /// @return const util::BitSet& Component mask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        constexpr const util::BitSet &getMask() const override final
        {
            return _mask;
        }

      private:
        std::unordered_map<Entity::Index, Component> _components;
        util::BitSet _mask;
    };
} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_MAPSTORAGE_HPP_ */
