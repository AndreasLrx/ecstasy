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

#include "IStorage.hpp"
#include "ecstasy/resources/entity/Entity.hpp"
#include "util/BitSet.hpp"

namespace ecstasy
{
    ///
    /// @brief Linear vector to store entity components.
    /// Recommended for dense components. (ie. components that are attached to all entities)
    ///
    /// @note This storage is not recommended for sparse components as it will waste memory.
    /// @warning Requires default constructible and movable components (for the padding elements).
    ///
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-19)
    ///
    template <typename C>
    class VectorStorage : public IStorage {
      public:
        /// @brief IsStorage constraint
        using Component = C;

        /// @brief @ref ecstasy::query::QueryableObject constraint.
        using QueryData = C &;
        /// @brief @ref ecstasy::query::ConstQueryableObject constraint.
        using ConstQueryData = const C &;

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

        ///
        /// @brief Erase the @b Component instance associated to the given entity.
        ///
        /// @note Does nothing if the index doesn't match with any component (ie if the entity doesn't have a component
        /// @b Component)
        /// @note Unset the flag in the mask but effectively delete the component only if it's the last one. Otherwise,
        /// it becomes a padding element.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return bool True if the component was erased, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        bool erase(Entity::Index index)
        {
            if (index >= _components.size())
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
            // We must do explicit check because the index can be a padding element
            if (!contains(index))
                throw std::out_of_range("Entity doesn't have the component");
            return _components[index];
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
            // We must do explicit check because the index can be a padding element
            if (!contains(index))
                throw std::out_of_range("Entity doesn't have the component");
            return _components[index];
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
            // We must do explicit check because the index can be a padding element
            if (!contains(index))
                throw std::out_of_range("Entity doesn't have the component");
            return _components[index];
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
            // We must do explicit check because the index can be a padding element
            if (!contains(index))
                throw std::out_of_range("Entity doesn't have the component");
            return _components[index];
        }

        ///
        /// @brief Test if the entity index match a @b Component instance.
        ///
        /// @param[in] index Index of the entity.
        ///
        /// @return bool True if the entity has a component, false otherwise.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        bool contains(Entity::Index index) const
        {
            return (index < _components.size()) && _mask[index];
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
        std::vector<Component> _components;
        util::BitSet _mask;

        friend class VectorStorageTest;
    };
} // namespace ecstasy

#endif /* !ECSTASY_STORAGE_VECTORSTORAGE_HPP_ */
