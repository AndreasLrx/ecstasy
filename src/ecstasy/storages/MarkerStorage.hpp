///
/// @file MarkerStorage.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-04-22
///
/// @copyright Copyright (c) ECSTASY 2024 042224
///
///

#ifndef ECSTASY_STORAGE_MARKERSTORAGE_HPP_
#define ECSTASY_STORAGE_MARKERSTORAGE_HPP_

#include "IStorage.hpp"
#include "ecstasy/resources/entity/Entity.hpp"
#include "util/BitSet.hpp"

namespace ecstasy
{
    ///
    /// @brief Storage for empty components. This storage stores the presence of the component instead of its data.
    /// The returned value is always a reference to a default constructed component.
    ///
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-22)
    ///
    template <typename C>
    class MarkerStorage : public IStorage {
      public:
        /// @brief IsStorage constraint
        using Component = C;

        /// @brief @ref ecstasy::query::QueryableObject constraint.
        using QueryData = C &;
        /// @brief @ref ecstasy::query::ConstQueryableObject constraint.
        using ConstQueryData = const C &;

        ///
        /// @brief Construct a new Marker Storage for a given Component type.
        ///
        /// @tparam Args Type of the arguments to forward to the component constructor.
        ///
        /// @param[in] args Args to forward to the component constructor.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        template <typename... Args>
        MarkerStorage(Args &&...componentArgs) : _defaultComponent(std::forward<Args>(componentArgs)...)
        {
        }

        ///
        /// @brief Copy constructor is deleted.
        ///
        /// @param[in] other Storage to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        MarkerStorage(const MarkerStorage &other) = delete;

        ///
        /// @brief Set the marker for a given entity.
        ///
        /// @note No check is done to see if the entity already has the marker.
        ///
        /// @param[in] index Entity index.
        ///
        /// @return Component& Placeholder component.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        Component &emplace(Entity::Index index)
        {
            _mask.resize(std::max(_mask.size(), index + 1));
            _mask[index] = true;
            return _defaultComponent;
        }

        ///
        /// @brief Erase the marker of the given entity.
        ///
        /// @note Does nothing if the entity doesn't have the marker.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return bool True if the marker was erased, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        bool erase(Entity::Index index)
        {
            if (_mask.size() <= index)
                return false;
            bool result = _mask[index];

            _mask[index] = false;
            return result;
        }

        ///
        /// @brief Erase the marker from multiple @p entities.
        ///
        /// @note Does nothing for entity without the marker.
        ///
        /// @param[in] entities target entities.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        void erase(std::span<Entity> entities) override final
        {
            for (Entity entity : entities)
                erase(entity.getIndex());
        }

        ///
        /// @brief Retrieve the default marker if the entity is marked.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return const Component& Const reference to the internal marker.
        ///
        /// @throw std::out_of_range If the entity doesn't have the marker.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        const Component &operator[](Entity::Index index) const
        {
            if (!this->contains(index))
                throw std::out_of_range("Entity doesn't have the marker.");
            return _defaultComponent;
        }

        /// @copydoc MarkerStorage::operator[]
        Component &operator[](Entity::Index index)
        {
            if (!this->contains(index))
                throw std::out_of_range("Entity doesn't have the marker.");
            return _defaultComponent;
        }

        ///
        /// @brief Retrieve the default marker if the entity is marked.
        ///
        /// @note @ref ecstasy::query::QueryableObject constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return Component& Reference to the default marker.
        ///
        /// @throw std::out_of_range If the entity doesn't have the marker.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        Component &getQueryData(Entity::Index index)
        {
            if (!this->contains(index))
                throw std::out_of_range("Entity doesn't have the marker.");
            return _defaultComponent;
        }

        ///
        /// @brief Retrieve the default marker if the entity is marked.
        ///
        /// @note @ref ecstasy::query::ConstQueryableObject constraint.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return const Component& Const reference to the default marker.
        ///
        /// @throw std::out_of_range If the entity doesn't have the marker.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        const Component &getQueryData(Entity::Index index) const
        {
            if (!this->contains(index))
                throw std::out_of_range("Entity doesn't have the marker.");
            return _defaultComponent;
        }

        ///
        /// @brief Test if the entity index is marked.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return bool True if the entity is marked, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        bool contains(Entity::Index index) const
        {
            return (index < _mask.size()) && _mask[index];
        }

        ///
        /// @brief Get the marker Mask.
        ///
        /// @note Each bit set to true mean the entity at the bit index is marked.
        /// @note @ref ecstasy::query::QueryableObject constraint.
        /// @warning The mask might be smaller than the entity count.
        ///
        /// @return const util::BitSet& Marker mask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        constexpr const util::BitSet &getMask() const override final
        {
            return _mask;
        }

        ///
        /// @brief Get the internal Component returned for all entities.
        ///
        /// @return constexpr const Component& Const reference to the internal marker.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        constexpr const Component &GetInternalComponent() const
        {
            return _defaultComponent;
        }

        ///
        /// @brief Get the Internal Component returned for all entities.
        ///
        /// @return constexpr Component& Reference to the internal marker.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-22)
        ///
        constexpr Component &GetInternalComponent()
        {
            return _defaultComponent;
        }

      private:
        Component _defaultComponent;
        util::BitSet _mask;
    };
} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_MARKERSTORAGE_HPP_ */
