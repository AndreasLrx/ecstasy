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

#include "AStorage.hpp"

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
    class MarkerStorage : public AStorage<C> {
      public:
        /// @brief IsStorage constraint
        using Component = typename AStorage<C>::Component;

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

        /// @copydoc insert
        Component &insert(Entity::Index index, Component &&c) override final
        {
            (void)c;
            _mask.resize(std::max(_mask.size(), index + 1));
            _mask[index] = true;
            return _defaultComponent;
        }

        /// @copydoc AStorage::erase(Entity::Index)
        bool erase(Entity::Index index) override final
        {
            if (_mask.size() <= index)
                return false;
            bool result = _mask[index];

            _mask[index] = false;
            return result;
        }

        /// @copydoc AStorage::operator[]
        Component &operator[](Entity::Index index) noexcept override final
        {
            (void)index;
            return _defaultComponent;
        }

        /// @copydoc AStorage::operator[]
        const Component &operator[](Entity::Index index) const noexcept override final
        {
            (void)index;
            return _defaultComponent;
        }

        /// @copydoc IStorage::getMask
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
