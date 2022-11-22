///
/// @file Registry.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_REGISTRY_HPP_
#define ECSTASY_REGISTRY_REGISTRY_HPP_

#include <optional>
#include <span>

#include "concepts/ComponentType.hpp"
#include "concepts/QueryableType.hpp"
#include "concepts/RegistryModifier.hpp"
#include "ecstasy/query/Query.hpp"
#include "ecstasy/query/Select.hpp"
#include "ecstasy/query/modifiers/Modifier.hpp"
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/storages/IStorage.hpp"
#include "ecstasy/storages/Instances.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"
#include "ecstasy/system/ISystem.hpp"
#include "util/Allocator.hpp"
#include "util/meta/outer_join.hpp"

namespace ecstasy
{
    using ModifiersAllocator = util::Allocator<ecstasy::query::modifier::Modifier>;
    using OptionalModifiersAllocator = std::optional<std::reference_wrapper<ModifiersAllocator>>;
    class Resource;

    ///
    /// @brief Base of an ECS architecture. It stores the entities, components and systems.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    class Registry {
      private:
        ///
        /// @brief Get a queryable from a registry variable (component stoage, resource, queryable storage...)
        ///
        /// @warning If the type is (or might be) a modifier that will have to be allocated, you must send a @ref
        /// ModifiersAllocator as parameter.
        ///
        /// @tparam C Type of the variable to fetch.
        ///
        /// @param[in] allocator Allocator to instanciate query modifier. Can be empty if no modifier needs to be
        /// instanciated.
        ///
        /// @return @ref getStorageType<C>& Associated queryable (if no specific case the storage for C is
        /// returned).
        ///
        /// @throw std::logic_error When @p allocator is empty and the queryable needs to be allocated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-25)
        ///
        template <typename C>
        constexpr getStorageType<C> &getQueryable(OptionalModifiersAllocator &allocator)
        {
            (void)allocator;
            return getStorageSafe<C>();
        }

        /// @copydoc getQueryable()
        template <std::derived_from<Resource> R>
        requires query::Queryable<R>
        constexpr R &getQueryable(OptionalModifiersAllocator &allocator)
        {
            (void)allocator;
            return getResource<R>();
        }

        /// @copydoc getQueryable()
        template <IsStorage S>
        requires query::Queryable<S> S &getQueryable(OptionalModifiersAllocator &allocator)
        {
            (void)allocator;
            if (!_storages.contains<S>())
                return _storages.emplace<S>();
            return _storages.get<S>();
        }

        ///
        /// @brief Proxy structure to extract the operand types using template partial specialization
        ///
        /// @tparam Operands Must be a tuple of @ref Queryable types.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-22)
        ///
        template <typename Operands>
        struct GetModifierProxy {
        };

        ///
        /// @brief Proxy structure to extract the operand types using template partial specialization
        ///
        /// @tparam Qs Operands types of the modifier.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-22)
        ///
        template <query::Queryable... Qs>
        struct GetModifierProxy<std::tuple<Qs...>> {
            ///
            /// @brief Specialization of @ref Registry::getQueryable(). Returns the required modifier with the operands
            /// queried from the registry.
            ///
            /// @tparam M Modifier type.
            ///
            /// @param[in] registry Owning registry.
            /// @param[in] allocator Allocator to instanciate the modifier. Must not be empty.
            ///
            /// @return M& Required modifier.
            ///
            /// @throw std::logic_error When @p allocator is empty.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-11-22)
            ///
            template <query::Modifier M>
            static constexpr M &get(Registry &registry, OptionalModifiersAllocator &allocator)
            {
                if (!allocator)
                    throw std::logic_error("Missing modifier allocator");
                return allocator->get().instanciate<M>(registry.getQueryable<Qs>(allocator)...);
            }
        };

        /// @copydoc getQueryable()
        template <query::Modifier M>
        constexpr M &getQueryable(OptionalModifiersAllocator &allocator)
        {
            return GetModifierProxy<typename M::Operands>::template get<M>(*this, allocator);
        }

        /// @copydoc getQueryable()
        template <RegistryModifier M>
        constexpr typename M::Modifier &getQueryable(OptionalModifiersAllocator &allocator)
        {
            return getQueryable<typename M::Modifier>(allocator);
        }

      public:
        ///
        /// @brief Entity Builder using the registry storages.
        ///
        /// @note Use Registry::entityBuilder() to create an instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        class EntityBuilder {
          public:
            ///
            /// @brief Copy constructor is deleted.
            ///
            /// @param[in] other Builder to copy.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            EntityBuilder(const EntityBuilder &other) = delete;

            ///
            /// @brief Add a component to the builder target entity.
            ///
            /// @tparam C Component type.
            /// @tparam Args Type of the Component constructor parameters
            ///
            /// @param[in] args Arguments to forward to the component constructor.
            ///
            /// @return EntityBuilder& @b this.
            ///
            /// @throw std::logic_error If the builder was already consumed or if the entity already has the
            /// component.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            template <typename C, typename... Args>
            EntityBuilder &with(Args &&...args)
            {
                _builder.with(_registry.getStorageSafe<C>(), std::forward<Args>(args)...);
                return *this;
            }

            ///
            /// @brief Finalize the entity, making it alive.
            ///
            /// @return Entity Newly created entity.
            ///
            /// @throw std::logic_error If the builder was already consumed.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            Entity build();

          private:
            Registry &_registry;
            Entities::Builder _builder;

            ///
            /// @brief Construct a new EntityBuilder, this method can only be called by a @ref Registry.
            ///
            /// @param[in] builder Internal Entities builder.
            /// @param[in] entity Entity target (modified by the builder).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            EntityBuilder(Registry &registry);

            friend Registry;
        };

        ///
        /// @brief Proxy class to use @ref where method.
        ///
        /// @note This is a registry helper for the @ref ecstasy::query::Select implementation.
        ///
        /// @tparam Selects type of selected queryables.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-22)
        ///
        template <query::Queryable... Selects>
        class Select {
          private:
            ///
            /// @brief Internal structure allowing to add implicitly required queryables (from the selected types).
            ///
            /// @tparam MissingsTuple Tuple type wrapping all the missing queryable types in the request. (they will be
            /// implicitly added).
            /// @tparam Cs Queryables already in the where clause.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-26)
            ///
            template <typename MissingsTuple, typename... Cs>
            struct Internal;

            template <typename... Missings, typename... Cs>
            struct Internal<std::tuple<Missings...>, Cs...> {
                constexpr static query::Query<Selects...> where(
                    Registry &registry, OptionalModifiersAllocator &allocator)
                {
                    if constexpr (sizeof...(Missings) > 0)
                        return ecstasy::query::Select<Selects...>::where(
                            registry.getQueryable<Missings>(allocator)..., registry.getQueryable<Cs>(allocator)...);
                    else
                        return ecstasy::query::Select<Selects...>::where(registry.getQueryable<Cs>(allocator)...);
                }
            };

          public:
            ///
            /// @brief Construct a new Select object.
            ///
            /// @param[in] registry registry owning the searched components.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            Select(Registry &registry) : _registry(registry)
            {
            }

            ///
            /// @brief Query all entities which have all the given components.
            ///
            /// @note If you need to use modifiers you must send a @ref ModifiersAllocator reference.
            ///
            /// @tparam C First component Type.
            /// @tparam Cs Other component Types.
            ///
            /// @param[in] allocator Allocator for the modifiers.
            ///
            /// @return @ref query::Query<Selects...> Resulting query.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            template <typename C, typename... Cs>
            query::Query<Selects...> where(OptionalModifiersAllocator allocator = std::nullopt)
            {
                // clang-format off
                return Internal<
                    typename util::meta::right_outer_join_t<
                        util::meta::Traits<
                            queryable_type_t<C>,
                            queryable_type_t<Cs>...
                        >,
                        util::meta::Traits<
                            Selects...
                        >
                    >::Tuple,
                    C, Cs...>::where(_registry, allocator);
                // clang-format on
            }

          private:
            Registry &_registry;
        };

        ///
        /// @brief Construct a new Registry.
        ///
        /// @param[in] addEntities Whether or not the @ref Entities @ref Resource should be declared or not in the
        /// registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        Registry(bool addEntities = true);

        /// @brief Default destructor.
        ~Registry() = default;

        ///
        /// @brief Create a new entity builder.
        ///
        /// @return EntityBuilder Newly created builder.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        [[nodiscard]] EntityBuilder entityBuilder();

        ///
        /// @brief Add a new system in the registry.
        ///
        /// @tparam S System to add.
        /// @tparam Args The type of arguments to pass to the constructor of @b S.
        ///
        /// @param[in] args The arguments to pass to the constructor of @b S.
        ///
        /// @return S& newly created System.
        ///
        /// @throw std::logic_error If the system @b S was already present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> S, typename... Args>
        S &addSystem(Args &&...args)
        {
            return _systems.emplace<S>(std::forward<Args>(args)...);
        }

        ///
        /// @brief Add a new resource in the registry.
        ///
        /// @tparam R Resource to add.
        /// @tparam Args The type of arguments to pass to the constructor of @b R.
        ///
        /// @param[in] args The arguments to pass to the constructor of @b R.
        ///
        /// @return R& newly created Resource.
        ///
        /// @throw std::logic_error If the resource @b R was already present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <std::derived_from<Resource> R, typename... Args>
        R &addResource(Args &&...args)
        {
            return _resources.emplace<R>(std::forward<Args>(args)...);
        }

        ///
        /// @brief Add a new component storage in the registry.
        ///
        /// @tparam C Component type to register.
        ///
        /// @return @ref getStorageType<C>& newly created Storage.
        ///
        /// @throw std::logic_error If a storage for component @b C was already present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <typename C>
        getStorageType<C> &addStorage()
        {
            return _storages.emplace<getStorageType<C>>();
        }

        ///
        /// @brief Check whether the registry has the resource of type @b R.
        ///
        /// @tparam R Type of the resource.
        ///
        /// @return bool Whether the registry contains a resource instance of type @b R.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-06)
        ///
        template <std::derived_from<Resource> R>
        bool hasResource() const
        {
            return _resources.contains<R>();
        }

        ///
        /// @brief Get the Resource of type @b R.
        ///
        /// @tparam R Type of the resource to fetch.
        ///
        /// @return const R& Const reference to an instance of type @b R.
        ///
        /// @throw std::logic_error If the resource @b R was not present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <std::derived_from<Resource> R>
        const R &getResource() const
        {
            return _resources.get<R>();
        }

        ///
        /// @brief Get the Resource of type @b R.
        ///
        /// @tparam R Type of the resource to fetch.
        ///
        /// @return R& Reference to an instance of type @b R.
        ///
        /// @throw std::logic_error If the resource @b R was not present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <std::derived_from<Resource> R>
        R &getResource()
        {
            return _resources.get<R>();
        }

        ///
        /// @brief Get the Storage for the component type @b C.
        ///
        /// @tparam C Type of the component for which we want the storage.
        ///
        /// @return @ref getStorageType<C>& Const reference to the storage of the component type @b C.
        ///
        /// @throw std::logic_error If no storage for component @b C was found in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <typename C>
        const getStorageType<C> &getStorage() const
        {
            return _storages.get<getStorageType<C>>();
        }

        ///
        /// @brief Get the Storage for the component type @b C.
        ///
        /// @tparam C Type of the component for which we want the storage.
        ///
        /// @return @ref getStorageType<C>& Reference to the storage of the component type @b C.
        ///
        /// @throw std::logic_error If no storage for component @b C was found in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <typename C>
        getStorageType<C> &getStorage()
        {
            return _storages.get<getStorageType<C>>();
        }

        ///
        /// @brief Get the Storage of a component and create it if not found.
        ///
        /// @tparam C Type of the comonent for which we want the storage.
        ///
        /// @return @ref getStorageType<C>& Reference to the storage of the component type @b C.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-19)
        ///
        template <typename C>
        getStorageType<C> &getStorageSafe() noexcept
        {
            if (!_storages.contains<getStorageType<C>>())
                addStorage<C>();
            return _storages.get<getStorageType<C>>();
        }

        ///
        /// @brief Get the Entities resource.
        ///
        /// @return const Entities& Const reference to the @ref Entities resource.
        ///
        /// @throw std::logic_error If the resource is not found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        const Entities &getEntities() const;

        ///
        /// @brief Get the Entities resource.
        ///
        /// @return Entities& Reference to the @ref Entities resource.
        ///
        /// @throw std::logic_error If the resource is not found.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        Entities &getEntities();

        ///
        /// @brief Get the Entity at the index @p index.
        ///
        /// @note The entity returned may not exists.
        ///
        /// @param[in] index Entity index.
        ///
        /// @return Entity matching entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        Entity getEntity(Entity::Index index) noexcept;

        ///
        /// @brief Construct a query for the given components.
        ///
        /// @note @p allocator is required when you use modifiers.
        ///
        /// @tparam C First component type.
        /// @tparam Cs Other component types.
        ///
        /// @param[in] allocator Allocator for the required modifiers (Maybe...).
        ///
        /// @return @ref query::Query<@ref queryable_type_t<C>, @ref queryable_type_t<Cs>...> New query which can be
        /// iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        template <typename C, typename... Cs>
        query::Query<queryable_type_t<C>, queryable_type_t<Cs>...> query(
            OptionalModifiersAllocator allocator = std::nullopt)
        {
            return query::Query(getQueryable<C>(allocator), getQueryable<Cs>(allocator)...);
        }

        ///
        /// @brief Starts the creation of a complex query in the registry.
        ///
        /// @note It does nothing until the @ref Select::where() method is called.
        ///
        /// @tparam C First component to query.
        /// @tparam Cs Other components to query.
        ///
        /// @return @ref Select<@ref queryable_type_t<C>, @ref queryable_type_t<Cs>...> placeholder templated Select
        /// object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-24)
        ///
        template <typename C, typename... Cs>
        Select<queryable_type_t<C>, queryable_type_t<Cs>...> select()
        {
            return Select<queryable_type_t<C>, queryable_type_t<Cs>...>(*this);
        }

        ///
        /// @brief Instantly erase an entity and its components from the registry.
        ///
        /// @param[in] entity @ref Entity to erase.
        ///
        /// @return bool Whether or not the entity was erased.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        bool eraseEntity(Entity entity);

        ///
        /// @brief Instantly erase multiple entities and their components from the registry.
        ///
        /// @param[in] entities entities to erase.
        ///
        /// @return size_t number of erased entities.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        size_t eraseEntities(std::span<Entity> entities);

        ///
        /// @brief Run a specific system from the registry.
        ///
        /// @tparam S System class to run.
        ///
        /// @throw std::logic_error If the system @b S is not present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        template <std::derived_from<ISystem> S>
        void runSystem()
        {
            _systems.get<S>().run(*this);
        }

        ///
        /// @brief Run all systems present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        void runSystems();

      private:
        Instances<ISystem> _systems;
        Instances<Resource> _resources;
        Instances<IStorage> _storages;

        /// @internal
        /// @brief Erase all the @p entity components.
        ///
        /// @note Prefer use of @ref eraseEntitiesComponents when possible.
        ///
        /// @param[in] entity Target entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        void eraseEntityComponents(Entity entity);

        /// @internal
        /// @brief Erase all the @p entities components.
        ///
        /// @param[in] entities Target entities.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-21)
        ///
        void eraseEntitiesComponents(std::span<Entity> entities);
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_REGISTRY_HPP_ */
