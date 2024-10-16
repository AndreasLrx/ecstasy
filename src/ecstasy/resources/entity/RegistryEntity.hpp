///
/// @file RegistryEntity.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Entity containing a reference to the registry. Allows to find storage implicitly.
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_RESOURCE_ENTITY_REGISTRYENTITY_HPP_
#define ECSTASY_RESOURCE_ENTITY_REGISTRYENTITY_HPP_

#include "Entity.hpp"
#include "ecstasy/registry/Registry.hpp"

namespace ecstasy
{
    ///
    /// @brief Entity containing a reference to the registry. Allows to find storage implicitly.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-19)
    ///
    class RegistryEntity : public Entity {
      public:
        ///
        /// @brief Construct a new RegistryEntity by binding an @ref Entity to a @ref Registry reference.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        constexpr RegistryEntity(Entity entity, Registry &registry) noexcept
            : Entity(entity.getIndex(), entity.getGeneration()), _registry(registry)
        {
        }

        ///
        /// @brief Add a component to the entity.
        ///
        /// @tparam C Component type.
        /// @tparam Args Type of the arguments to forward to the component constructor.
        ///
        /// @param[in] args Arguments to forward to the component constructor.
        ///
        /// @return C& Reference to the newly created component.
        ///
        /// @throw std::logic_error If the component was already present.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <typename C, typename... Args>
        C &add(Args &&...args)
        {
            return Entity::add(_registry.getStorageSafe<C>(), std::forward<Args>(args)...);
        }

        ///
        /// @brief Try to fetch the instance of component @b C associated to the current entity.
        ///
        /// @tparam C Type of the component to retrieve.
        ///
        /// @return const C& Const reference to the entity instance of @b C associated to the entity.
        ///
        /// @throw std::out_of_range If no associated instance found.
        /// @throw std::logic_error If the storage is not registered.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <typename C>
        [[nodiscard]] const C &get() const
        {
            return _registry.getStorage<C>()[_index];
        }

        ///
        /// @brief Try to fetch the instance of component @b C associated to the current entity.
        ///
        /// @tparam C Type of the component to retrieve.
        ///
        /// @return C& Reference to the entity instance of @b C associated to the entity.
        ///
        /// @throw std::out_of_range If no associated instance found.
        /// @throw std::logic_error If the storage is not registered.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <typename C>
        [[nodiscard]] C &get()
        {
            return _registry.getStorage<C>()[_index];
        }

        ///
        /// @brief Test if the entity has an associated component in the storage @b S.
        ///
        /// @tparam S Storage of a component.
        ///
        /// @return bool True if the entity has an associated entry in the storage.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <typename C>
        [[nodiscard]] bool has() const
        {
            return _registry.getStorageSafe<C>().contains(_index);
        }

        ///
        /// @brief Get the entity owning registry.
        ///
        /// @return Registry& Reference to the entity owning registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        [[nodiscard]] constexpr Registry &getRegistry() noexcept
        {
            return _registry;
        }

      private:
        /// @brief Reference to the registry.
        Registry &_registry;
    };
} // namespace ecstasy

#endif /* !ECSTASY_RESOURCE_ENTITY_REGISTRYENTITY_HPP_ */
