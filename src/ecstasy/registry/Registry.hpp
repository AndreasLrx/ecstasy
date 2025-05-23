///
/// @file Registry.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Registry class definition.
/// @version 1.0.0
/// @date 2022-10-17
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_REGISTRY_HPP_
#define ECSTASY_REGISTRY_REGISTRY_HPP_

#include <optional>
#include <span>

#include "concepts/RegistryModifier.hpp"
#include "ecstasy/query/Query.hpp"
#include "ecstasy/query/Select.hpp"
#include "ecstasy/query/conditions/Condition.hpp"
#include "ecstasy/query/modifiers/Modifier.hpp"
#include "ecstasy/resources/entity/Entities.hpp"
#include "ecstasy/storages/IStorage.hpp"
#include "ecstasy/storages/Instances.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"
#include "ecstasy/storages/SystemInstances.hpp"
#include "ecstasy/system/ISystem.hpp"
#include "ecstasy/system/Pipeline.hpp"
#include "ecstasy/thread/LockableView.hpp"
#include "util/Allocator.hpp"
#include "util/StackAllocator.hpp"
#include "util/meta/apply.hpp"
#include "util/meta/filter.hpp"
#include "concepts/component_type.hpp"
#include "concepts/queryable_type.hpp"
#include "ecstasy/registry/concepts/modifier_allocator_size.hpp"
#include "util/meta/is_size_t_convertible.hpp"
#include "util/meta/outer_join.hpp"

#ifdef _MSC_VER
    #define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
    #define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

namespace ecstasy
{

    ///
    /// @brief Resource reference type.
    /// This type is used to reference a resource in a thread-safe way depending on the AutoLock parameter.
    ///
    /// @note Consider this as a thread_safe @ref std::reference_wrapper.
    ///
    /// @tparam R Resource type.
    /// @tparam AutoLock Whether the resource should be locked automatically.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <typename R, bool AutoLock = thread::AUTO_LOCK_RESOURCES_DEFAULT>
    using ResourceReference = query::thread_safe_reference_t<R, AutoLock>;
    /// @brief @ref ResourceReference alias
    template <typename R, bool AutoLock = thread::AUTO_LOCK_RESOURCES_DEFAULT>
    using RR = ResourceReference<R, AutoLock>;

    /// @internal
    /// @brief Empty type used with no_unique_address attribute to avoid memory overhead.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-07)
    ///
    struct EmptyType {};

    ///
    /// @brief Base of an ECS architecture. It stores the entities, components and systems.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-18)
    ///
    class Registry {
      public:
        ///
        /// @brief Get a queryable from a registry variable (component storage, resource, queryable storage...)
        ///
        /// @tparam C Type of the variable to fetch.
        ///
        /// @return @ref getStorageType<C>& Associated queryable (if no specific case the storage for C is
        /// returned).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-25)
        ///
        template <typename C>
        [[nodiscard]] constexpr getStorageType<C> &getQueryable()
        {
            return getStorageSafe<C>();
        }

        /// @copydoc getQueryable()
        template <std::derived_from<IResource> R>
            requires query::Queryable<R>
        [[nodiscard]] constexpr R &getQueryable()
        {
            return getResource<R, false>();
        }

        /// @copydoc getQueryable()
        template <IsStorage S>
            requires query::Queryable<S>
        [[nodiscard]] S &getQueryable()
        {
            if (!_storages.contains<std::remove_const_t<S>>()) [[unlikely]]
                return _storages.emplace<std::remove_const_t<S>>();
            return _storages.get<std::remove_const_t<S>>();
        }

        ///
        /// @brief Get a registry object reference (storage, resource, system, or the registry itself) from its type.
        ///
        /// @tparam C Type of the registry object to fetch.
        ///
        /// @return constexpr getStorageType<C>& Associated registry object (if no specific case the storage for C is
        /// returned).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-03)
        ///
        template <typename C>
        [[nodiscard]] constexpr getStorageType<C> &getFromType()
        {
            return getStorageSafe<C>();
        }

        /// @copydoc getFromType()
        template <IsStorage S>
        [[nodiscard]] S &getFromType()
        {
            return _storages.get<std::remove_const_t<S>>();
        }

        /// @copydoc getFromType()
        template <std::derived_from<IResource> R>
        [[nodiscard]] constexpr R &getFromType()
        {
            return getResource<R, false>();
        }

        /// @copydoc getFromType()
        template <std::derived_from<ISystem> S>
        [[nodiscard]] constexpr S &getFromType()
        {
            return getSystem<S>();
        }

        /// @copydoc getFromType()
        template <std::derived_from<Registry> R>
        [[nodiscard]] constexpr R &getFromType() noexcept
        {
            return *this;
        }

        ///
        /// @brief Proxy structure to extract the operand types using template partial specialization
        ///
        /// @tparam Operands Must be a tuple of @ref ecstasy::query::Queryable "Queryable" types.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-22)
        ///
        template <typename Operands>
        struct GetModifierProxy {};

        ///
        /// @brief Proxy structure to extract the operand types using template partial specialization
        ///
        /// @tparam Qs Operands types of the modifier.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-11-22)
        ///
        template <query::Queryable... Qs>
        struct GetModifierProxy<util::meta::Traits<Qs...>> {
            ///
            /// @brief Specialization of @ref Registry::getQueryable(). Returns the required modifier with the
            /// operands queried from the registry.
            ///
            /// @tparam M Modifier type.
            /// @tparam ModifierAllocator Modifiers allocator type.
            ///
            /// @param[in] registry Owning registry.
            /// @param[in] allocator Allocator to instanciate the modifier. Must not be empty.
            ///
            /// @return M& Required modifier.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-11-22)
            ///
            template <query::Modifier M, typename ModifierAllocator>
            [[nodiscard]] static constexpr M &get(Registry &registry, ModifierAllocator &allocator)
            {
                return allocator.template instanciate<M>(
                    getRegistryQueryable<Qs, ModifierAllocator>(registry, allocator)...);
            }

          private:
            ///
            /// @brief Get a queryable from the registry. This function is a proxy to send or not the allocator
            ///
            /// @tparam Q Queryable type to get.
            /// @tparam ModifierAllocator Allocator type for the modifier.
            ///
            /// @param[in] registry Owning registry.
            /// @param[in] allocator Allocator to instanciate the modifier (if required).
            ///
            /// @return constexpr Q& Reference to the queryable (maybe in a view/modifier).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-04-17)
            ///
            template <typename Q, typename ModifierAllocator>
            [[nodiscard]] static constexpr Q &getRegistryQueryable(Registry &registry, ModifierAllocator &allocator)
            {
                if constexpr (RegistryModifier<Q> || query::Modifier<Q>)
                    return registry.getQueryable<Q, ModifierAllocator>(allocator);
                else
                    return registry.getQueryable<Q>();
            }
        };

        /// @copydoc getQueryable()
        template <query::Modifier M, typename ModifierAllocator>
        [[nodiscard]] constexpr M &getQueryable(ModifierAllocator &allocator)
        {
            return GetModifierProxy<typename M::Operands>::template get<M, ModifierAllocator>(*this, allocator);
        }

        /// @copydoc getQueryable()
        template <RegistryModifier M, typename ModifierAllocator>
        [[nodiscard]] constexpr typename M::Modifier &getQueryable(ModifierAllocator &allocator)
        {
            return getQueryable<typename M::Modifier, ModifierAllocator>(allocator);
        }

        ///
        /// @brief Base class of @ref RegistryStackQuery. This class is used to allocate the queryables on the stack.
        /// At compile time it will evaluate the required queryables and reserve memory for the one requiring an
        /// allocation. It allows faster runtime because there is no dynamic allocation but longer compile time.
        ///
        /// @note Thanks to no_unique_address attribute, the memory reserved for the allocator is not reserved if no
        /// allocator is required, meaning there is no memory overhead.
        /// @note Yes this is black magic and I'm proud of it (my brain hurts).
        ///
        /// @tparam Selects Selected queryables in a @ref util::meta::Traits.
        /// @tparam Missings Selected queryables not given in the where clause (in  @b Cs... ) in a @ref
        /// util::meta::Traits.
        /// @tparam Conditions Query runtime conditions in a @ref util::meta::Traits.
        /// @tparam AutoLock Whether or not the @ref ecstasy::thread::Lockable "Lockable" queryables must be locked.
        /// @tparam Cs Selected components already present in the where clause. (Missings + Cs are all the components
        /// queried).
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-07)
        ///
        template <typename Selects, typename Missings, typename Conditions, bool AutoLock, typename... Cs>
        class RegistryStackQueryMemory {};

        /// @copydoc RegistryStackQueryMemory
        template <typename... Selects, typename... Missings, typename... Conditions, bool AutoLock, typename... Cs>
        class RegistryStackQueryMemory<util::meta::Traits<Selects...>, util::meta::Traits<Missings...>,
            util::meta::Traits<Conditions...>, AutoLock, Cs...> {
          public:
            // clang-format off
                /// @brief Size in bytes of the modifiers allocator. 0 if no modifier.
                using ModifiersAllocatorSize =  modifiers_allocator_size<Missings..., Cs...>;
                /// @brief Whether or not the query has a modifier allocator.
                using HasModifiersAllocator = std::bool_constant<ModifiersAllocatorSize::value != 0>;
                /// @brief Type of the modifiers allocator.
                using ModifiersAllocator = std::conditional_t<
                    HasModifiersAllocator::value,
                    util::StackAllocator<ModifiersAllocatorSize::value, query::modifier::ModifierBase>,
                    EmptyType
                >;
                
                /// @brief Size in bytes of the views allocator. 0 if no view.
                using ViewsAllocatorSize = ecstasy::query::views_allocator_size<AutoLock, queryable_type_t<Missings>..., queryable_type_t<Cs>...>;
                /// @brief Whether or not the query has a views allocator.
                using HasViewsAllocator = std::bool_constant<ViewsAllocatorSize::value != 0>;
                /// @brief Type of the views allocator.
                using ViewsAllocator = std::conditional_t<
                    AutoLock && HasViewsAllocator::value,
                    util::StackAllocator<ViewsAllocatorSize::value, thread::LockableViewBase>,
                    EmptyType
                >;

                ///
                /// @brief Get the type of a queryable in this registry query context (with or without lock).
                ///
                /// @note This is a helper to get the queryable type with or without lock.
                /// 
                /// @tparam Q Queryable type (or component).
                ///
                /// @author Andréas Leroux (andreas.leroux@epitech.eu)
                /// @since 1.0.0 (2024-04-07)
                ///
                template <typename Q>
                using QueryableType = ecstasy::query::thread_safe_queryable_t<queryable_type_t<Q>, AutoLock>;

            // clang-format on

            ///
            /// @brief Construct a new Registry Stack Query Memory.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-04-07)
            ///
            RegistryStackQueryMemory(){};

            ///
            /// @brief Get a reference to a queryable from the registry. If the queryable needs to be allocated (view or
            /// modifier), allocate it and returns a reference to the allocated queryable.
            ///
            /// @tparam Q Queryable type to get.
            ///
            /// @param[in] registry Registry owning the queryable.
            ///
            /// @return constexpr QueryableType<Q>& Reference to the queryable (maybe in a view/modifier).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-04-07)
            ///
            template <typename Q>
            [[nodiscard]] constexpr QueryableType<Q> &getQueryable(Registry &registry)
            {
                if constexpr (AutoLock && HasViewsAllocator::value && thread::Lockable<queryable_type_t<Q>>)
                    return _viewsAllocator.template instanciate<thread::LockableView<queryable_type_t<Q>>>(
                        getRegistryQueryable<Q>(registry));
                else
                    return getRegistryQueryable<Q>(registry);
            }

          protected:
            /// @brief Modifiers allocator. Is @ref EmptyType if no modifier.
            NO_UNIQUE_ADDRESS ModifiersAllocator _modifiersAllocator;
            /// @brief Views allocator. Is @ref EmptyType if no view.
            NO_UNIQUE_ADDRESS ViewsAllocator _viewsAllocator;

          private:
            ///
            /// @brief Get a registry queryable. This is a proxy to call the right function with or without an
            /// allocator.
            ///
            /// @tparam Q Queryable type to get.
            /// @param[in] registry Owning registry.
            ///
            /// @return constexpr auto& Reference to the queryable (maybe in a view/modifier).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-04-17)
            ///
            template <typename Q>
            [[nodiscard]] constexpr auto &getRegistryQueryable(Registry &registry)
            {
                if constexpr (HasModifiersAllocator::value && (RegistryModifier<Q> || query::Modifier<Q>))
                    return registry.getQueryable<Q, ModifiersAllocator>(_modifiersAllocator);
                else
                    return registry.getQueryable<Q>();
            }
        };

        ///
        /// @brief Registry query allocating everything on the stack (if allocation required). This means longer compile
        /// time for faster runtime.
        ///
        /// @tparam Selects Selected queryables
        /// @tparam Missings Selected queryables not given in the where clause
        /// @tparam Condition Query runtime first condition
        /// @tparam Cs Query selected components already present in the where clause
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2023-11-08)
        ///
        template <typename Selects, typename Missings, typename Condition, typename Cs,
            bool AutoLock = thread::AUTO_LOCK_DEFAULT>
        class RegistryStackQuery {};

        // clang-format on

        template <bool AutoLock, typename... Selects>
        // clang-format off
        using RegistrySelectStackQueryMemory = RegistryStackQueryMemory<
            util::meta::Traits<Selects...>,
            util::meta::Traits<>,
            util::meta::Traits<>,
            AutoLock,
            Selects...
        >;

        template <bool AutoLock, typename... Selects>
        // clang-format off
        using RegistrySelectStackQuery = RegistryStackQuery<
            util::meta::Traits<Selects...>,
            util::meta::Traits<>,
            util::meta::Traits<>,
            util::meta::Traits<Selects...>,
            AutoLock
        >;
        // clang-format on

        ///
        /// @brief Registry query allocating everything on the stack (if allocation required). This means longer compile
        /// time for faster runtime.
        ///
        /// The goal is to return a single object containing the query object and the modifiers/views allocator. Thus
        /// the lifetime of the query and of the modifiers/views are bound on the stack.
        ///
        /// This was quite difficult to achieve since the RegistryStackQuery needs the allocator and the query, but the
        /// query needs the allocator...
        /// The solution I found is using multiple inheritance to call a function before the query creation.
        /// In our case the function is the @ref RegistryStackQueryMemory constructor. This ensure a safe allocator
        /// initialization just before the query creation and without move operations (meaning memory copy and usually
        /// pointers invalidation).
        ///
        /// @tparam Selects Selected queryables
        /// @tparam Missings Selected queryables not given in the where clause
        /// @tparam Condition Query runtime first condition
        /// @tparam Cs Query selected components already present in the where clause
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2023-11-08)
        ///
        template <typename... Selects, typename... Missings, typename Conditions, typename... Cs, bool AutoLock>
        class RegistryStackQuery<util::meta::Traits<Selects...>, util::meta::Traits<Missings...>, Conditions,
            util::meta::Traits<Cs...>, AutoLock>
            // clang-format off
            : public RegistryStackQueryMemory<
                        util::meta::Traits<Selects...>, 
                        util::meta::Traits<Missings...>,
                        Conditions, 
                        AutoLock, 
                        Cs...>,
              public query::QueryImplementation<util::meta::Traits<
                    typename RegistryStackQueryMemory<
                        util::meta::Traits<Selects...>, 
                        util::meta::Traits<Missings...>,
                        Conditions, 
                        AutoLock, 
                        Cs...>::template QueryableType<Selects>...>,
                    Conditions> 
            {
              public:
                /// @brief Memory type for the query.
                using Memory = RegistryStackQueryMemory<
                    util::meta::Traits<Selects...>, 
                    util::meta::Traits<Missings...>,
                    Conditions, 
                    AutoLock, 
                    Cs...>;
                /// @brief Query type for the query.
                using Base = ecstasy::query::QueryImplementation<
                    util::meta::Traits<typename Memory::template QueryableType<Selects>...>, 
                    Conditions>;
                

                ///
                /// @brief Construct a new Registry Stack Query.
                ///
                /// @param[in] registry owning registry to query the queryables (of course).
                ///
                /// @author Andréas Leroux (andreas.leroux@epitech.eu)
                /// @since 1.0.0 (2023-11-08)
                ///
                RegistryStackQuery(Registry & registry)
                    : Memory(),
                      Base(std::move(
                        ecstasy::query::Select<
                            typename Memory::template QueryableType<Selects>...>::template 
                        where<Conditions,
                             typename Memory::template QueryableType<Missings>...,
                             typename Memory::template QueryableType<Cs>...
                        >(
                            Memory::template getQueryable<Missings>(registry)...,
                            Memory::template getQueryable<Cs>(registry)...
                        )))
                {
                }

            // clang-format on
        };

        ///
        /// @copydoc RegistryStackQuery
        ///
        template <typename... Selects, bool AutoLock>
        class RegistryStackQuery<util::meta::Traits<Selects...>, util::meta::Traits<>, util::meta::Traits<>,
            util::meta::Traits<Selects...>, AutoLock>
            // clang-format off
            : public RegistrySelectStackQueryMemory<AutoLock, Selects...>,
              public query::Query<
                typename RegistrySelectStackQueryMemory<AutoLock, Selects...>::template QueryableType<Selects>...
                >
            {
          public:
            /// @brief Memory type for the query.
            using Memory = RegistrySelectStackQueryMemory<AutoLock, Selects...>;
            /// @brief Query type for the query.
            using Base = query::Query<
                typename Memory::template QueryableType<Selects>...
            >;

            ///
            /// @brief Construct a new Registry Stack Query.
            ///
            /// @param[in] registry owning registry to query the queryables (of course).
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2023-11-08)
            ///
            RegistryStackQuery(Registry &registry)
                : Memory(),
                    Base(Memory::template getQueryable<Selects>(registry)...)
            {
            }
        };
        

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
            template <typename C, typename... Qs, typename... Args>
            EntityBuilder &with(Args &&...args)
            {
                _builder.with(_registry.getStorageSafe<C>(), _registry.getFromType<Qs>()..., std::forward<Args>(args)...);
                return *this;
            }

            ///
            /// @brief Add a component to the builder target entity.
            ///
            /// @tparam C Component type.
            ///
            /// @param[in] list Initializer list to forward to the component constructor.
            ///
            /// @return EntityBuilder& @b this.
            ///
            /// @throw std::logic_error If the builder was already consumed or if the entity already has the
            /// component.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2023-01-31)
            ///
            template <typename C>
                requires requires() { typename C::value_type; }
            EntityBuilder &with(std::initializer_list<typename C::value_type> list)
            {
                _builder.with(_registry.getStorageSafe<C>(), list);
                return *this;
            }

            ///
            /// @brief Finalize the entity, making it alive.
            ///
            /// @note The builder is consumed after this call. Any further with/build call will throw an exception.
            ///
            /// @return Entity Newly created entity.
            ///
            /// @throw std::logic_error If the builder was already consumed.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            Entity build();

            /// @copydoc Entities::Builder::getEntity
            [[nodiscard]] const Entity &getEntity() const noexcept
            {
                return _builder.getEntity();
            }

            /// @brief Get a const reference to the registry.
            ///
            /// @return const Registry& Const reference to the registry
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-08-27)
            ///
            [[nodiscard]] constexpr const Registry &getRegistry() const noexcept
            {
                return _registry;
            }

            /// @brief Get a reference to the registry.
            ///
            /// @return Registry& reference to the registry
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-08-27)
            ///
            [[nodiscard]] constexpr Registry &getRegistry() noexcept
            {
                return _registry;
            }

          private:
            /// @brief Owning registry.
            Registry &_registry;
            /// @brief Entities builder.
            Entities::Builder _builder;

            ///
            /// @brief Construct a new EntityBuilder, this method can only be called by a @ref Registry.
            ///
            /// @param[in] registry Parent registry.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-19)
            ///
            EntityBuilder(Registry &registry) noexcept;

            friend Registry;
        };

        ///
        /// @brief Proxy class to use @ref where method.
        ///
        /// @note This is a registry helper for the @ref ecstasy::query::Select "Select" implementation.
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
            /// @brief Test whether a given type is a condition
            ///
            /// @tparam T type to evaluate
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2023-11-08)
            ///
            template <typename T>
            struct IsCondition : public std::is_base_of<ecstasy::query::ConditionBase, T> {};

            ///
            /// @brief Test whether a given type is not a condition
            ///
            /// @tparam T type to evaluate
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2023-11-08)
            ///
            template <typename T>
            struct IsNotCondition : std::integral_constant<bool, !IsCondition<T>::value> {};

            // clang-format off
            /// @brief @ref util::meta::Traits type helper for the Selected queryable template pack
            using SelectsTraits = util::meta::Traits<Selects...>;
            /// @brief @ref util::meta::Traits type helper for the missing queryable from a given template pack
            template <typename... Cs>
            using MissingsTraits = util::meta::right_outer_join_t<
                util::meta::apply_t<queryable_type_t, 
                    util::meta::filter_t<IsNotCondition, Cs...>>,
                    util::meta::Traits<Selects...>
                >;
            /// @brief @ref util::meta::Traits type helper for the components types from a given template pack
            template <typename... Cs>
            using ComponentsTraits = util::meta::filter_t<IsNotCondition, Cs...>;
            /// @brief @ref util::meta::Traits type helper for the conditions types from a given template pack
            template <typename... Cs>
            using ConditionsTraits = util::meta::filter_t<IsCondition, Cs...>;
            /// @brief @ref query::QueryImplementation type for the given where clause
            template <typename... Cs>
            using QueryType = query::QueryImplementation<SelectsTraits, ConditionsTraits<Cs...>>;
            // clang-format on

          public:
            ///
            /// @brief Construct a new Select object.
            ///
            /// @param[in] registry registry owning the searched components.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            Select(Registry &registry) noexcept : _registry(registry)
            {
            }

            ///
            /// @brief Query all entities which have all the given components.
            ///
            /// @tparam C First constraint Type (Queryable or Condition).
            /// @tparam Cs Other constraint Types (Queryables or Conditions).
            ///
            /// @return @ref QueryType<C, Cs...> Resulting query.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            // clang-format off
            template <typename C, typename... Cs>
            [[nodiscard]] RegistryStackQuery<
                        SelectsTraits,
                        MissingsTraits<C, Cs...>,
                        ConditionsTraits<C, Cs...>,
                        ComponentsTraits<C, Cs...>
                    >
            where()
            {
                    return RegistryStackQuery<
                        SelectsTraits,
                        MissingsTraits<C, Cs...>,
                        ConditionsTraits<C, Cs...>,
                        ComponentsTraits<C, Cs...>
                    >(_registry);
            }

            ///
            /// @brief Query all entities which have all the given components. This extended version allows to set the AutoLock value.
            /// 
            /// @tparam AutoLock Whether or not the queryables should be locked.
            /// @tparam C First constraint Type (Queryable or Condition).
            /// @tparam Cs Other constraint Types (Queryables or Conditions).
            ///
            /// @return RegistryStackQuery Query object which can be iterated.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2024-04-07)
            ///
            template <bool AutoLock, typename C, typename... Cs>
            [[nodiscard]] RegistryStackQuery<
                        SelectsTraits,
                        MissingsTraits<C, Cs...>,
                        ConditionsTraits<C, Cs...>,
                        ComponentsTraits<C, Cs...>,
                        AutoLock
                    >
            whereEx()
            {
                    return RegistryStackQuery<
                        SelectsTraits,
                        MissingsTraits<C, Cs...>,
                        ConditionsTraits<C, Cs...>,
                        ComponentsTraits<C, Cs...>,
                        AutoLock
                    >(_registry);
            }
            // clang-format on

          private:
            /// @brief Owning registry.
            Registry &_registry;
        };

        ///
        /// @brief Construct a new Registry.
        ///
        /// @param[in] addEntities Whether or not the @ref Entities @ref IResource should be declared or not in the
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
        [[nodiscard]] EntityBuilder entityBuilder() noexcept;

        ///
        /// @brief Add a new system in the registry.
        ///
        /// @note Call @ref addSystemInPhase to add a system in a specific phase using runtime phase id or an Enum type
        /// for phase id.
        ///
        /// @tparam S System to add.
        /// @tparam Phase System phase. See @ref Pipeline.
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
        template <std::derived_from<ISystem> S,
            Pipeline::PhaseId Phase = static_cast<Pipeline::PhaseId>(Pipeline::PredefinedPhases::OnUpdate),
            typename... Args>
        S &addSystem(Args &&...args)
        {
            return addSystemInPhase<S>(Phase, std::forward<Args>(args)...);
        }

        ///
        /// @brief Add a new system in the registry in a specific phase.
        ///
        /// @tparam S System to add.
        /// @tparam T Phase id type. Usually an enum convertible to size_t, or a size_t directly.
        /// @tparam Args The type of arguments to pass to the constructor of @b S.
        ///
        /// @param[in] phaseId Phase id where to add the system.
        /// @param[in] args The arguments to pass to the constructor of @b S.
        ///
        /// @return S& newly created System.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        template <std::derived_from<ISystem> S, util::meta::is_size_t_convertible T, typename... Args>
        S &addSystemInPhase(T phaseId, Args &&...args)
        {
            S &system = _systems.emplace<S>(std::forward<Args>(args)...);

            _pipeline.addSystem(typeid(S), static_cast<size_t>(phaseId));
            return system;
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
        template <std::derived_from<IResource> R, typename... Args>
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
        template <std::derived_from<IResource> R>
        [[nodiscard]] bool hasResource() const
        {
            return _resources.contains<R>();
        }

        ///
        /// @brief Get the Resource of type @b R.
        ///
        /// @tparam R Type of the resource to fetch.
        /// @tparam Locked Whether or not the resource should be locked.
        ///
        /// @return const R& Const reference to an instance of type @b R.
        ///
        /// @throw std::logic_error If the resource @b R was not present in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-18)
        ///
        template <std::derived_from<IResource> R, bool Locked = thread::AUTO_LOCK_RESOURCES_DEFAULT>
        [[nodiscard]] ResourceReference<const R, Locked> getResource() const
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
        template <std::derived_from<IResource> R, bool Locked = thread::AUTO_LOCK_RESOURCES_DEFAULT>
        [[nodiscard]] ResourceReference<R, Locked> getResource()
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
        [[nodiscard]] const getStorageType<C> &getStorage() const
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
        [[nodiscard]] getStorageType<C> &getStorage()
        {
            return _storages.get<std::remove_const_t<getStorageType<C>>>();
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
        [[nodiscard]] getStorageType<C> &getStorageSafe() noexcept
        {
            if (!_storages.contains<std::remove_const_t<getStorageType<C>>>()) [[unlikely]]
                addStorage<std::remove_const_t<C>>();
            return _storages.get<getStorageType<std::remove_const_t<C>>>();
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
        template <bool Locked = thread::AUTO_LOCK_RESOURCES_DEFAULT>
        [[nodiscard]] ResourceReference<const Entities, Locked> getEntities() const
        {
            return _resources.get<Entities>();
        }

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
        template <bool Locked = thread::AUTO_LOCK_RESOURCES_DEFAULT>
        [[nodiscard]] ResourceReference<Entities, Locked> getEntities()
        {
            return _resources.get<Entities>();
        }

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
        [[nodiscard]] Entity getEntity(Entity::Index index);

        ///
        /// @brief Get the Entity Storages
        ///
        /// @note This is the only way to return a list of components attached to an entity because components have no
        /// base class constraints and therefore cannot be stored in a container.
        ///
        /// @param[in] entity Entity to get the storages from.
        ///
        /// @return std::vector<std::reference_wrapper<IStorage>> List of storages attached to the entity.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        [[nodiscard]] std::vector<std::reference_wrapper<IStorage>> getEntityStorages(Entity entity);

        ///
        /// @brief Get the System of type @b S.
        ///
        /// @tparam S Type of the system to get.
        ////
        /// @return S& Reference to the system of type @b S.
        ///
        /// @throw std::logic_error If the system @b S was not found in the registry.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-03)
        ///
        template <std::derived_from<ISystem> S>
        [[nodiscard]] S &getSystem()
        {
            return _systems.get<S>();
        }

        ///
        /// @brief Checks whether there is an instance of system S in the registry.
        ///
        /// @tparam S ISystem type.
        ///
        /// @author Andréas Leroux
        /// @since 1.0.0 (2025-05-02)
        ///
        template <std::derived_from<ISystem> S>
        [[nodiscard]] bool hasSystem()
        {
            return _systems.contains<S>();
        }

        ///
        /// @brief Construct a query for the given components.
        ///
        /// @tparam C First component type.
        /// @tparam Cs Other component types.
        ///
        /// @return @ref query::Query<@ref queryable_type_t<C>, @ref queryable_type_t<Cs>...> New query which can be
        /// iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        // clang-format off
        template <typename C, typename... Cs>
        [[nodiscard]] RegistrySelectStackQuery<thread::AUTO_LOCK_DEFAULT, queryable_type_t<C>, queryable_type_t<Cs>...>
        query()
        {
            return RegistrySelectStackQuery<thread::AUTO_LOCK_DEFAULT, queryable_type_t<C>, queryable_type_t<Cs>...>(*this);
        }


        ///
        /// @brief Construct a query for the given components. This extented version allows to explicitly set the AutoLock value.
        /// 
        /// @tparam AutoLock Whether or not the query should be auto locked.
        /// @tparam C First component type.
        /// @tparam Cs Other component types.
        ///
        /// @return RegistrySelectStackQuery<AutoLock, queryable_type_t<C>, queryable_type_t<Cs>...> New query which can be iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-07)
        ///
        template <bool AutoLock, typename C, typename... Cs>
        [[nodiscard]] RegistrySelectStackQuery<AutoLock, queryable_type_t<C>, queryable_type_t<Cs>...>
        queryEx()
        {
            return RegistrySelectStackQuery<AutoLock, queryable_type_t<C>, queryable_type_t<Cs>...>(*this);
        }
        // clang-format on

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
        [[nodiscard]] Select<queryable_type_t<C>, queryable_type_t<Cs>...> select()
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
        /// @brief Erase all resources, systems and storages.
        ///
        /// @note Re creates the @ref Entities resource.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2023-02-06)
        ///
        void clear();

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
        /// @brief Run a specific system from the registry.
        ///
        /// @param[in] systemId System type index to run.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        void runSystem(const std::type_index &systemId);

        ///
        /// @brief Run all systems present in the registry.
        ///
        /// @note Systems are run in ascending registration order. You can have further control using
        /// @ref ecstasy::Pipeline::Phase "Phases".
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        void runSystems();

        ///
        /// @brief Run all systems present in the registry for the given phase.
        ///
        /// @param[in] phase Phase to run the systems for.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        void runSystemsPhase(Pipeline::PhaseId phase);

        ///
        /// @brief Get a const reference to the storages instances.
        ///
        /// @return constexpr const Instances<IStorage>& Const reference to the storages instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-06-11)
        ///
        [[nodiscard]] constexpr const Instances<IStorage> &getStorages() const noexcept
        {
            return _storages;
        }

        ///
        /// @brief Get a reference to the storages instances.
        ///
        /// @return constexpr Instances<IStorage>& Reference to the storages instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-04)
        ///
        [[nodiscard]] constexpr Instances<IStorage> &getStorages() noexcept
        {
            return _storages;
        }

        ///
        /// @brief Get a reference to the registry pipeline.
        ///
        /// @return Pipeline& Reference to the registry pipeline instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        [[nodiscard]] constexpr Pipeline &getPipeline() noexcept
        {
            return _pipeline;
        }

        ///
        /// @brief Get a const reference to the registry pipeline.
        ///
        /// @return const Pipeline& Const reference to the registry pipeline instance.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-11-21)
        ///
        [[nodiscard]] constexpr const Pipeline &getPipeline() const noexcept
        {
            return _pipeline;
        }

      private:
        /// @brief Registry resources.
        Instances<IResource> _resources;
        /// @brief Registry storages.
        Instances<IStorage> _storages;
        /// @brief Registry systems.
        Instances<ISystem> _systems;
        /// @brief System pipeline.
        Pipeline _pipeline;

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
