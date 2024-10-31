///
/// @file MapStorage.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Associative Map to store entity components
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_STORAGE_MAPSTORAGE_HPP_
#define ECSTASY_STORAGE_MAPSTORAGE_HPP_

#include <unordered_map>

#include "AStorage.hpp"

namespace ecstasy
{
    ///
    /// @brief Associative Map to store entity components
    /// Recommended for sparse components. (ie. components that are not attached to all entities)
    ///
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-19)
    ///
    template <typename C>
    class MapStorage : public AStorage<C> {
      public:
        using Component = typename AStorage<C>::Component;
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

        /// @copydoc AStorage::insert
        Component &insert(Entity::Index index, Component &&c) override final
        {
            if constexpr (!std::movable<Component>)
                throw std::runtime_error("MapStorage: Component is not movable");
            else {
                _mask.resize(std::max(_mask.size(), index + 1));
                _mask[index] = true;
                return _components.emplace(std::make_pair(index, std::move(c))).first->second;
            }
        }

        /// @copydoc AStorage::erase(Entity::Index)
        bool erase(Entity::Index index) override final
        {
            auto it = _components.find(index);

            if (it != _components.end()) [[likely]] {
                _components.erase(index);
                _mask[index] = false;
                return true;
            }
            return false;
        }

        /// @copydoc AStorage::operator[]
        [[nodiscard]] Component &operator[](Entity::Index index) override final
        {
            return _components.at(index);
        }

        /// @copydoc AStorage::operator[]
        [[nodiscard]] const Component &operator[](Entity::Index index) const override final
        {
            return _components.at(index);
        }

        ///
        /// @brief Get the number of @b Component instances.
        ///
        /// @return size_t number of @b Component instances.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        [[nodiscard]] size_t size() const noexcept
        {
            return _components.size();
        }

        /// @copydoc IStorage::getMask
        [[nodiscard]] constexpr const util::BitSet &getMask() const noexcept override final
        {
            return _mask;
        }

      private:
        /// @brief Components map.
        std::unordered_map<Entity::Index, Component> _components;
        /// @brief Component mask.
        util::BitSet _mask;
    };
} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_MAPSTORAGE_HPP_ */
