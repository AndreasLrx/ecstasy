///
/// @file Registry.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
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

#include "concepts/ComponentType.hpp"
#include "concepts/QueryableType.hpp"
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
#include "util/Allocator.hpp"
#include "util/StackAllocator.hpp"
#include "util/meta/apply.hpp"
#include "util/meta/filter.hpp"
#include "ecstasy/registry/concepts/modifier_allocator_size.hpp"
#include "util/meta/outer_join.hpp"

#ifdef ECSTASY_MULTI_THREAD
    #include "ecstasy/thread/LockableView.hpp"
#endif

#ifdef _MSC_VER
    #define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
    #define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

namespace ecstasy
{
    using ModifiersAllocator = util::Allocator<ecstasy::query::modifier::ModifierBase>;
    template <typename... Qs>
    using StackAllocator = util::StackAllocator<modifiers_allocator_size_v<Qs...>, query::modifier::ModifierBase>;
    template <typename A = ModifiersAllocator>
    using OptionalModifiersAllocator = std::optional<std::reference_wrapper<A>>;
    class ResourceBase;

    ///
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
        template <typename C, typename A = ModifiersAllocator>
        constexpr getStorageType<C> &getQueryable(OptionalModifiersAllocator<A> &allocator)
        {
            (void)allocator;
            return getStorageSafe<C>();
        }

        /// @copydoc getQueryable()
        template <std::derived_from<Resource> R, typename A = ModifiersAllocator>
            requires query::Queryable<R>
        constexpr R &getQueryable(OptionalModifiersAllocator<A> &allocator)
        {
            (void)allocator;
            return getResource<R>();
        }

        /// @copydoc getQueryable()
        template <IsStorage S, typename A = ModifiersAllocator>
            requires query::Queryable<S>
        S &getQueryable(OptionalModifiersAllocator<A> &allocator)
        {
            (void)allocator;
            if (!_storages.contains<std::remove_const_t<S>>())
                return _storages.emplace<std::remove_const_t<S>>();
            return _storages.get<std::remove_const_t<S>>();
        }

        ///
        /// @brief Proxy structure to extract the operand types using template partial specialization
        ///
        /// @tparam Operands Must be a tuple of @ref ecstasy::query::Queryable types.
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
            /// @tparam A Modifiers allocator type.
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
            template <query::Modifier M, typename A = ModifiersAllocator>
            static constexpr M &get(Registry &registry, OptionalModifiersAllocator<A> &allocator)
            {
                if (!allocator)
                    throw std::logic_error("Missing modifier allocator");
                return allocator->get().template instanciate<M>(registry.getQueryable<Qs>(allocator)...);
            }
        };

        /// @copydoc getQueryable()
        template <query::Modifier M, typename A = ModifiersAllocator>
        constexpr M &getQueryable(OptionalModifiersAllocator<A> &allocator)
        {
            return GetModifierProxy<typename M::Operands>::template get<M>(*this, allocator);
        }

        /// @copydoc getQueryable()
        template <RegistryModifier M, typename A = ModifiersAllocator>
        constexpr typename M::Modifier &getQueryable(OptionalModifiersAllocator<A> &allocator)
        {
            return getQueryable<typename M::Modifier>(allocator);
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
        /// @tparam AutoLock Whether or not the @ref thread::Lockable queryables must be locked.
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
                ///
                /// @brief Reference to the modifiers allocator.
                ///
                /// @note Since a reference to the allocator is always required for getQueryable, we use a default type 
                /// even if we don't need an allocator. 
                /// The getQueryable method should be changed to avoid requiring an allocator if not required.
                /// 
                /// @author Andréas Leroux (andreas.leroux@epitech.eu)
                /// @since 1.0.0 (2024-04-07)
                ///
                using ModifiersAllocatorReference = std::conditional_t<
                    HasModifiersAllocator::value,
                    std::optional<std::reference_wrapper<ModifiersAllocator>>,
                    std::optional<std::reference_wrapper<util::Allocator<ecstasy::query::modifier::ModifierBase>>>
                >;
                
#ifdef ECSTASY_MULTI_THREAD
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
#endif

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
            RegistryStackQueryMemory()
            {
                if constexpr (HasModifiersAllocator::value)
                    _modifiersAllocRef = _modifiersAllocator;
                else
                    _modifiersAllocRef = std::nullopt;
            };

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
            constexpr QueryableType<Q> &getQueryable(Registry &registry)
            {
#ifdef ECSTASY_MULTI_THREAD
                if constexpr (HasViewsAllocator::value && thread::Lockable<queryable_type_t<Q>>)
                    return _viewsAllocator.template instanciate<thread::LockableView<queryable_type_t<Q>>>(
                        registry.getQueryable<Q>(_modifiersAllocRef));
                else {
#endif
                    if constexpr (HasModifiersAllocator::value)
                        return registry.getQueryable<Q, ModifiersAllocator>(_modifiersAllocRef);
                    else

                        return registry.getQueryable<Q>(_modifiersAllocRef);
#ifdef ECSTASY_MULTI_THREAD
                }
#endif
            }

          protected:
            NO_UNIQUE_ADDRESS ModifiersAllocator _modifiersAllocator;
#ifdef ECSTASY_MULTI_THREAD
            NO_UNIQUE_ADDRESS ViewsAllocator _viewsAllocator;
#endif
            ModifiersAllocatorReference _modifiersAllocRef;
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
            bool AutoLock = THREAD_SAFE_DEFAULT>
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

        template <typename... Selects>
        // clang-format off
        using RegistrySelectStackQuery = RegistryStackQuery<
            util::meta::Traits<Selects...>,
            util::meta::Traits<>,
            util::meta::Traits<>,
            util::meta::Traits<Selects...>
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
            template <typename C, typename... Args>
            EntityBuilder &with(Args &&...args)
            {
                _builder.with(_registry.getStorageSafe<C>(), std::forward<Args>(args)...);
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
            /// @param[in] registry Parent registry.
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

            ///
            /// @brief Internal structure allowing to add implicitly required queryables (from the selected types).
            ///
            /// @tparam MissingsTuple Tuple type wrapping all the missing queryable types in the request. (they will
            /// be implicitly added).
            /// @tparam ComponentsTuples Queryables already in the where clause.
            /// @tparam ConditionsTuple Multiple @ref ecstasy::query::Condition to apply on query iteration.
            /// @tparam A Modifiers allocator type.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-26)
            ///
            template <typename Missings, typename Cs, typename Conditions, typename A = ModifiersAllocator>
            struct Internal;

            /// @copydoc Internal
            template <typename... Missings, typename... Cs, typename Conditions, typename A>
            struct Internal<util::meta::Traits<Missings...>, util::meta::Traits<Cs...>, Conditions, A> {
                ///
                /// @brief Return an instance of @ref query::QueryImplementation.
                ///
                /// @param[in] registry Registry instance to query all the required queryables.
                /// @param[in] allocator Modifiers allocator. Can be std::nullopt if no modifiers.
                ///
                /// @return constexpr query::QueryImplementation<SelectsTraits, Conditions> resulting query.
                ///
                /// @throw std::logic_error When @p allocator is empty and there is a modifier in the query.
                ///
                /// @author Andréas Leroux (andreas.leroux@epitech.eu)
                /// @since 1.0.0 (2023-11-08)
                ///
                constexpr static query::QueryImplementation<SelectsTraits, Conditions> where(
                    Registry &registry, OptionalModifiersAllocator<A> &allocator)
                {
                    return ecstasy::query::Select<Selects...>::template where<Conditions,
                        std::remove_reference_t<decltype(registry.getQueryable<Missings>(allocator))>...,
                        std::remove_reference_t<decltype(registry.getQueryable<Cs>(allocator))>...>(
                        registry.getQueryable<Missings, A>(allocator)..., registry.getQueryable<Cs, A>(allocator)...);
                }
            };

            ///
            /// @brief Query all entities which have all the given components.
            ///
            /// @note If you need to use modifiers you must send a @ref ModifiersAllocator reference.
            /// But if there is no allocator and one is required, this method will not be called, a @ref
            /// RegistryStackQuery will be made instead.
            ///
            /// @tparam A Modifiers allocator Type
            /// @tparam Cs Constraint Types (Queryables or Conditions).
            ///
            /// @param[in] allocator Allocator for the modifiers.
            ///
            /// @return @ref QueryType<Cs...> Resulting query.
            ///
            /// @throw std::logic_error When @p allocator is empty and there is a modifier in the query.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2023-11-08)
            ///
            template <typename A = ModifiersAllocator, typename... Cs>
            QueryType<Cs...> explicitWhere(OptionalModifiersAllocator<A> allocator = std::nullopt)
            {
                return Internal<MissingsTraits<Cs...>, ComponentsTraits<Cs...>, ConditionsTraits<Cs...>, A>::where(
                    _registry, allocator);
            }

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
            /// @note If no allocator is sent and one is required, the resulting query will be of type @ref
            /// RegistryStackQuery.
            ///
            /// @tparam C First constraint Type (Queryable or Condition).
            /// @tparam Cs Other constraint Types (Queryables or Conditions).
            /// @tparam A Modifiers allocator Type.
            ///
            /// @param[in] allocator Allocator for the modifiers.
            ///
            /// @return @ref QueryType<C, Cs...> Resulting query.
            ///
            /// @author Andréas Leroux (andreas.leroux@epitech.eu)
            /// @since 1.0.0 (2022-10-22)
            ///
            template <typename C, typename... Cs, typename A = ModifiersAllocator>
            QueryType<C, Cs...> where(OptionalModifiersAllocator<A> allocator)
            {
                return explicitWhere<A, C, Cs...>(allocator);
            }

            /// @copydoc where
            template <typename C, typename... Cs, typename A = ModifiersAllocator>
            constexpr QueryType<C, Cs...> where(A &allocator)
            {
                return explicitWhere<A, C, Cs...>(std::optional(std::reference_wrapper(allocator)));
            }

            // clang-format off
            /// @copydoc where
            template <typename C, typename... Cs>
            RegistryStackQuery<
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
            // clang-format on

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
        /// @tparam Priority System priority. See @ref Registry::runSystems().
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
        template <std::derived_from<ISystem> S, size_t Priority = 0, typename... Args>
        S &addSystem(Args &&...args)
        {
            return _systems.emplace<S, Priority>(std::forward<Args>(args)...);
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
        getStorageType<C> &getStorageSafe() noexcept
        {
            if (!_storages.contains<std::remove_const_t<getStorageType<C>>>())
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
        /// @note If you use modifers and don't send a @p allocator it will default to a RegistryStackQuery using a @ref
        /// StackAllocator.
        ///
        /// @tparam C First component type.
        /// @tparam Cs Other component types.
        /// @tparam A Modifiers allocator type.
        ///
        /// @param[in] allocator Allocator for the required modifiers (Maybe...).
        ///
        /// @return @ref query::Query<@ref queryable_type_t<C>, @ref queryable_type_t<Cs>...> New query which can be
        /// iterated.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-20)
        ///
        template <typename C, typename... Cs, typename A = ModifiersAllocator>
        query::Query<queryable_type_t<C>, queryable_type_t<Cs>...> query(OptionalModifiersAllocator<A> allocator)
        {
            return query::Query(getQueryable<C>(allocator), getQueryable<Cs>(allocator)...);
        }

        /// @copydoc query
        template <typename C, typename... Cs, typename A = ModifiersAllocator>
        constexpr query::Query<queryable_type_t<C>, queryable_type_t<Cs>...> query(A &allocator)
        {
            return query<C, Cs...>(std::optional(std::reference_wrapper(allocator)));
        }

        // clang-format off
        /// @copydoc query
        template <typename C, typename... Cs>
        RegistrySelectStackQuery<queryable_type_t<C>, queryable_type_t<Cs>...>
        query()
        {
            return RegistrySelectStackQuery<queryable_type_t<C>, queryable_type_t<Cs>...>(*this);
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
        /// @brief Run all systems present in the registry.
        ///
        /// @note Systems are run in ascending priority order. If two systems have the same priority, run order is
        /// undefined.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-10-17)
        ///
        void runSystems();

        ///
        /// @brief Run all systems whose priority match the given group.
        ///
        /// @note The system groups can be seen as an internet network: The @p group 'id' is the network address, the
        /// @p mask is the network mask and each system priority in the group is a host in the network range.
        ///
        /// @note Systems in the group are run in ascending priority order. If two systems have the same priority, run
        /// order is undefined.
        ///
        /// @param[in] group Group priority 'id'.
        /// @param[in] mask Priority 'id' mask.
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2022-12-19)
        ///
        void runSystems(size_t group, size_t mask);

      private:
        Instances<Resource> _resources;
        Instances<IStorage> _storages;
        SystemInstances _systems;

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
