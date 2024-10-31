///
/// @file Entities.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Definition of the Entities class.
/// @version 1.0.0
/// @date 2022-10-18
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_RESOURCE_ENTITY_ENTITIES_HPP_
#define ECSTASY_RESOURCE_ENTITY_ENTITIES_HPP_

#include <span>
#include <vector>

#include "Entity.hpp"
#include "ecstasy/resources/IResource.hpp"
#include "util/BitSet.hpp"

namespace ecstasy
{
    ///
    /// @brief Resource holding all the Registry entities.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-18)
    ///
    class Entities : public IResource {
      public:
        ///
        /// @brief Entities builder to add multiple component to an entity on creation.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        class Builder {
          public:
            ///
            /// @brief Cannot copy a builder.
            ///
            /// @param[in] other builder to copy.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            Builder(const Builder &other) = delete;

            ///
            /// @brief Add a component to the builder target entity.
            ///
            /// @tparam S Component storage type.
            /// @tparam Args Type of the Component constructor parameters
            ///
            /// @param[in] storage Component storage.
            /// @param[in] args Arguments to forward to the component constructor.
            ///
            /// @return Builder& @b this.
            ///
            /// @throw std::logic_error If the builder was already consumed or if the entity already has the
            /// component.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            template <IsStorage S, typename... Args>
            Builder &with(S &storage, Args &&...args)
            {
                assertNotBuilt();
                _entity.add(storage, std::forward<Args>(args)...);
                return *this;
            }

            ///
            /// @brief Add a component to the builder target entity.
            ///
            /// @tparam S Component storage type.
            ///
            /// @param[in] storage Component storage.
            /// @param[in] list Initializer list to forward to the component constructor.
            ///
            /// @return Builder& @b this.
            ///
            /// @throw std::logic_error If the builder was already consumed or if the entity already has the
            /// component.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2023-01-31)
            ///
            template <IsContainerStorage S>
            Builder &with(S &storage, std::initializer_list<typename S::Component::value_type> list)
            {
                assertNotBuilt();
                _entity.add(storage, list);
                return *this;
            }

            ///
            /// @brief Finalize the entity, making it alive.
            ///
            /// @note This method can only be called once.
            ///
            /// @return Entity Newly created entity.
            ///
            /// @throw std::logic_error If the builder was already consumed.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            Entity build();

            ///
            /// @brief Get a const reference to the entity being built.
            ///
            /// @warning This method is here in case you need the entity id before calling @ref build(). For example if
            /// you need it inside a component constructor.
            ///
            /// @return const Entity& Entity being built.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-06-07)
            ///
            [[nodiscard]] const Entity &getEntity() const noexcept
            {
                return _entity;
            }

          private:
            /// @brief Reference to the parent Entities.
            Entities &_parent;
            /// @brief Entity being built.
            Entity _entity;
            /// @brief Whether the builder has been consumed.
            bool _built;

            ///
            /// @brief Construct a new Builder, this method can only be called by an @ref Entities.
            ///
            /// @param[in] parent Entities object creating this builder.
            /// @param[in] entity Entity target (modified by the builder).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            Builder(Entities &parent, Entity entity) noexcept;

            ///
            /// @brief Verify if the builder has not already been consumed.
            ///
            /// @throw std::logic_error if @b _built is true.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            void assertNotBuilt() const;

            friend Entities;
        };

        /// @brief @ref ecstasy::query::QueryableObject "QueryableObject" constraint
        using QueryData = Entity;
        /// @brief @ref ecstasy::query::ConstQueryableObject "ConstQueryableObject" constraint
        using ConstQueryData = QueryData;

        ///
        /// @brief Construct a new Entities resource without any entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        Entities();

        ///
        /// @brief Create a new entity, alive or not.
        ///
        /// @param[in] alive Whether the entity must be created alive or not.
        ///
        /// @return Entity Newly created entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        Entity create(bool alive = true);

        ///
        /// @brief Create a new entity builder.
        ///
        /// @return Builder Newly created builder.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        [[nodiscard]] Builder builder();

        ///
        /// @brief Retrieve an entity from its identifier.
        ///
        /// @param[in] id Index of the entity.
        ///
        /// @return Entity
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        [[nodiscard]] Entity get(Entity::Index id) const noexcept;

        ///
        /// @brief Erase (delete) the entity when called.
        ///
        /// @note Entity is considered erased if it was alive.
        ///
        /// @param[in] entity Entity to erase.
        ///
        /// @return bool Whether the entity was erased or not.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        bool erase(Entity entity);

        ///
        /// @brief Erase multiple entities when called.
        ///
        /// @note An entity is considered erased if it was alive before.
        ///
        /// @param[in] entities entities to erase.
        ///
        /// @return size_t Number of entities erased.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        size_t erase(std::span<Entity> entities);

        ///
        /// @brief Mark en entity for deletion. The entity is alive until a call to @ref ecstasy::Entities::maintain()
        /// is made.
        ///
        /// @param[in] entity @ref Entity to kill.
        ///
        /// @return bool Whether or not the entity is marked for deletion.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        bool kill(Entity entity);

        ///
        /// @brief Tests if an entity is alive.
        ///
        /// @param[in] entity entity to test.
        ///
        /// @return bool If the entity is alive.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        [[nodiscard]] bool isAlive(Entity entity) const;

        ///
        /// @brief Get the Alive entities mask.
        ///
        /// @return const util::BitSet& alive entities mask.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        [[nodiscard]] constexpr const util::BitSet &getMask() const noexcept
        {
            return _alive;
        }

        ///
        /// @brief Get the entity from its index.
        ///
        /// @note @ref ecstasy::query::QueryableObject "QueryableObject" constraints.
        ///
        /// @return const Entity Entity at index @p index.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        [[nodiscard]] Entity getQueryData(Entity::Index index) const;

        ///
        /// @brief Effectively erase the entities marked for deletion (using @ref kill()).
        ///
        /// @note Invalidate iterators.
        /// @warning This function doesn't delete the entities components, see @ref Registry::maintain() instead.
        ///
        /// @return @ref std::vector<Entity> List of deleted entities.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        std::vector<Entity> maintain();

      private:
        /// @brief List of entity generations.
        std::vector<Entity::Generation> _generations;
        /// @brief Mask of alive entities.
        util::BitSet _alive;
        /// @brief Mask of entities marked for deletion.
        util::BitSet _killed;
    };
} // namespace ecstasy

#endif /* !ECSTASY_RESOURCE_ENTITY_ENTITIES_HPP_ */
