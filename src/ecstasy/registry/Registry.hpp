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

#include "concepts/ComponentType.hpp"
#include "concepts/QueryableType.hpp"
#include "concepts/RegistryModifier.hpp"
#include "ecstasy/query/Query.hpp"
#include "ecstasy/query/Select.hpp"
#include "ecstasy/query/concepts/GetMissingTypes.hpp"
#include "ecstasy/query/modifiers/Modifier.hpp"
#include "ecstasy/resource/entity/Entities.hpp"
#include "ecstasy/storage/IStorage.hpp"
#include "ecstasy/storage/Instances.hpp"
#include "ecstasy/storage/StorageConcepts.hpp"
#include "ecstasy/system/ISystem.hpp"
#include "util/Allocator.hpp"

#include <span>

namespace ecstasy
{
    using ModifiersAllocator = util::Allocator<ecstasy::query::modifier::Modifier>;
    class Resource;

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
        /// @return constexpr getStorageType<C>& Associated queryable (if no specific case the storage for C is
        /// returned).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-25)
        ///
        template <typename C>
        constexpr getStorageType<C> &getQueryable()
        {
            return getStorageSafe<C>();
        }

        /// @copydoc getQueryable()
        template <std::derived_from<Resource> R>
        requires query::Queryable<R>
        constexpr R &getQueryable()
        {
            return getResource<R>();
        }

        /// @copydoc getQueryable()
        template <IsStorage S>
        requires query::Queryable<S>
        constexpr S &getQueryable()
        {
            return _storages.get<S>();
        }

        /// @copydoc getQueryable()
        template <typename C>
        constexpr queryable_type_t<C> &getQueryable(ModifiersAllocator &allocator)
        {
            (void)allocator;
            return getQueryable<C>();
        }

        /// @copydoc getQueryable()
        template <std::derived_from<query::modifier::Modifier> M>
        requires query::Queryable<M>
        constexpr M &getQueryable(ModifiersAllocator &allocator)
        {
            return allocator.instanciate<M>(getQueryable<typename M::Internal>());
        }

        /// @copydoc getQueryable()
        template <RegistryModifier M>
        requires query::Queryable<typename M::Modifier>
        constexpr M::Modifier &getQueryable(ModifiersAllocator &allocator)
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
            /// @brief Construct a new EntityBuilder, this method can only be called by an @ref Registry.
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
        /// @brief Proxy class to use where static method.
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

            /// @copydoc
            ///
            /// @note This template is used when there is no missing queryables.
            template <typename... Cs>
            struct Internal<std::tuple<void>, Cs...> {
                constexpr static query::Query<Selects...> where(Registry &registry, ModifiersAllocator &allocator)
                {
                    return ecstasy::query::Select<Selects...>::where(registry.getQueryable<Cs>(allocator)...);
                }

                constexpr static query::Query<Selects...> where(Registry &registry)
                {
                    return ecstasy::query::Select<Selects...>::where(registry.getQueryable<Cs>()...);
                }
            };

            /// @copydoc
            ///
            /// @note This template is used when there are missing queryables whoses types are @p Missings.
            template <typename... Missings, typename... Cs>
            struct Internal<std::tuple<Missings...>, Cs...> {
                constexpr static query::Query<Selects...> where(Registry &registry, ModifiersAllocator &allocator)
                {
                    return ecstasy::query::Select<Selects...>::where(
                        registry.getQueryable<Missings>(allocator)..., registry.getQueryable<Cs>(allocator)...);
                }

                constexpr static query::Query<Selects...> where(Registry &registry)
                {
                    return ecstasy::query::Select<Selects...>::where(
                        registry.getQueryable<Missings>()..., registry.getQueryable<Cs>()...);
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
            /// @note If you don't use any modifiers, don't send the allocator.
            ///
            /// @tparam C First component Type.
            /// @tparam Cs Other component Types.
            ///
            /// @param[in] allocator Allocator for the modifiers.
            ///
            /// @return Query<Selects...> Resulting query.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            template <typename C, typename... Cs>
            query::Query<Selects...> where(ModifiersAllocator &allocator)
            {
                return Internal<typename ecstasy::query::available_types<queryable_type_t<C>,
                                    queryable_type_t<Cs>...>::template get_missings_t<Selects...>,
                    C, Cs...>::where(_registry, allocator);
            }

            ///
            /// @brief Query all entities which have all the given components.
            ///
            /// @note If you need to use modifiers you must send a @ref ModifiersAllocator reference.
            ///
            /// @tparam C First component Type.
            /// @tparam Cs Other component Types.
            ///
            /// @return Query<Selects...> Resulting query.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            template <typename C, typename... Cs>
            query::Query<Selects...> where()
            {
                return Internal<typename ecstasy::query::available_types<queryable_type_t<C>,
                                    queryable_type_t<Cs>...>::template get_missings_t<Selects...>,
                    C, Cs...>::where(_registry);
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
        /// @return getStorageType<C>& newly created Storage.
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
        /// @return getStorageType<C>& Const reference to the storage of the component type @b C.
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
        /// @return getStorageType<C>& Reference to the storage of the component type @b C.
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
        /// @return getStorageType<C>& Reference to the storage of the component type @b C.
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
        /// @tparam C First component type.
        /// @tparam Cs Other component types.
        ///
        /// @return Query<queryable_type_t<C>, queryable_type_t<Cs>...> New query which can be iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        template <typename C, typename... Cs>
        query::Query<queryable_type_t<C>, queryable_type_t<Cs>...> query()
        {
            return query::Query(getQueryable<C>(), getQueryable<Cs>()...);
        }

        ///
        /// @brief Construct a query for the given components.
        ///
        /// @note If your query doesn't use any modifier, you should omit the allocator parameter.
        ///
        /// @tparam C First component type.
        /// @tparam Cs Other component types.
        ///
        /// @param[in] allocator Allocator for the required modifiers (Maybe...).
        ///
        /// @return Query<queryable_type_t<C>, queryable_type_t<Cs>...> New query which can be iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        template <typename C, typename... Cs>
        query::Query<queryable_type_t<C>, queryable_type_t<Cs>...> query(ModifiersAllocator &allocator)
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
        /// @return Select<queryable_type_t<C>, queryable_type_t<Cs>...> placeholder templated Select object.
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
        /// @return size_t
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
