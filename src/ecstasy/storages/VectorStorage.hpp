///
/// @file VectorStorage.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_STORAGE_VECTORSTORAGE_HPP_
#define ECSTASY_STORAGE_VECTORSTORAGE_HPP_

#include <vector>

#include "AStorage.hpp"

namespace ecstasy
{
    ///
    /// @brief Linear vector to store entity components.
    /// Recommended for dense components. (ie. components that are attached to all entities)
    ///
    /// @note This storage is not recommended for sparse components as it will waste memory.
    /// @warning Requires default constructible (for the padding elements) and movable components (including move
    /// assignement operator).
    ///
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-19)
    ///
    template <typename C>
    class VectorStorage : public AStorage<C> {
      public:
        /// @brief IsStorage constraint
        using Component = typename AStorage<C>::Component;

        ///
        /// @brief Construct a new Vector Storage for a given Component type.
        ///
        /// @param[in] initialCapacity Initial capacity of the storage.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        VectorStorage(size_t initialCapacity = 0) : _components(), _mask()
        {
            if (initialCapacity) {
                _components.reserve(initialCapacity);
                _mask.resize(initialCapacity);
            }
        };

        ///
        /// @brief Copy constructor is deleted.
        ///
        /// @param[in] other Storage to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        VectorStorage(const VectorStorage &other) = delete;

        ///
        /// @brief Emplace a new @b Component instance for a given entity.
        ///
        /// @note Overwrite the previous component if any.
        /// @warning This method may resize the internal vector, which will probably invalidate references to the
        /// components.
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
            // Component at index already existing
            if (_components.size() > index)
                _components[index] = std::move(Component(std::forward<Args>(args)...));
            else {
                _mask.resize(std::max(_mask.size(), index + 1));
                // Padding elements
                if (_components.size() < index)
                    _components.resize(index);
                // New component
                _components.emplace_back(std::forward<Args>(args)...);
            }
            _mask[index] = true;
            return _components[index];
        }

        /// @copydoc insert
        Component &insert(Entity::Index index, Component &&c) override final
        {
            if constexpr (!std::movable<Component>)
                throw std::runtime_error("VectorStorage: Component is not movable");
            else {
                // Component at index already existing
                if (_components.size() > index)
                    _components[index] = std::move(c);
                else {
                    _mask.resize(std::max(_mask.size(), index + 1));
                    // Padding elements
                    if (_components.size() < index)
                        _components.resize(index);
                    // New component
                    _components.push_back(std::move(c));
                }
                _mask[index] = true;
                return _components[index];
            }
        }

        /// @copydoc AStorage::erase
        ///
        /// @note Unset the flag in the mask but effectively delete the component only if it's the last one. Otherwise,
        /// it becomes a padding element.
        bool erase(Entity::Index index) override final
        {
            if (index >= _components.size()) [[unlikely]]
                return false;
            size_t effectiveSize;

            _mask[index] = false;
            effectiveSize = _mask.lastSet() + 1;

            // Last component, we can safely remove it and the padding elements until the last set bit
            // The size is never lower than 1 because 0 is returned if no bit is set (unsigned number)
            if (effectiveSize < _mask.size()) {
                _mask.resize(effectiveSize);
                _components.resize(effectiveSize);
            }
            return true;
        }

        /// @copydoc AStorage::operator[]
        [[nodiscard]] Component &operator[](Entity::Index index) noexcept override final
        {
            return _components[index];
        }

        /// @copydoc AStorage::operator[]
        [[nodiscard]] const Component &operator[](Entity::Index index) const noexcept override final
        {
            return _components[index];
        }

        /// @copydoc IStorage::getMask
        [[nodiscard]] constexpr const util::BitSet &getMask() const noexcept override final
        {
            return _mask;
        }

      private:
        /// @brief Components vector.
        std::vector<Component> _components;
        /// @brief Mask to know if a component is set for an entity.
        util::BitSet _mask;

        friend class VectorStorageTest;
    };
} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_VECTORSTORAGE_HPP_ */
