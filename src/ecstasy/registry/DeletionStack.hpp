///
/// @file DeletionStack.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-21
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_DELETIONSTACK_HPP_
#define ECSTASY_REGISTRY_DELETIONSTACK_HPP_

#include <vector>

#include "ecstasy/resources/entity/Entity.hpp"

namespace ecstasy
{
    class Registry;

    ///
    /// @brief Helper to manage entity deletion safely within an iteration.
    /// @todo Find a better name.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-21)
    ///
    class DeletionStack {
      public:
        ///
        /// @brief Construct a new Deletion Stack attached to a given registry.
        ///
        /// @param[in] registry Registry attached with the entities to delete.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        DeletionStack(Registry &registry);

        ///
        /// @brief Deleted copy constructor.
        ///
        /// @param[in] other Stack to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        DeletionStack(const DeletionStack &other) = delete;

        ///
        /// @brief Destroy the Deletion Stack, calling @ref Registry::erase() with all the entity marked for deletion
        /// using @ref push().
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        ~DeletionStack();

        ///
        /// @brief Mark an entity for deletion upon destruction of this instance.
        ///
        /// @param[in] entity entity to delete.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        void push(Entity entity);

        ///
        /// @brief Return the number of entities marked for deletion.
        ///
        /// @return size_t number of entities marked for deletion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        size_t size() const;

      private:
        Registry &_registry;
        std::vector<Entity> _killed;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_DELETIONSTACK_HPP_ */
