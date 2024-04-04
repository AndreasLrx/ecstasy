///
/// @file QueryableType.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_CONCEPTS_QUERYABLETYPE_HPP_
#define ECSTASY_REGISTRY_CONCEPTS_QUERYABLETYPE_HPP_

#include "ecstasy/query/concepts/Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "ecstasy/registry/concepts/RegistryModifier.hpp"
#include "ecstasy/resources/Resource.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"

namespace ecstasy
{
    ///
    /// @brief Try to get the type of a queryable associated to a type stored in the registry (component, storage,
    /// resource...).
    /// Returns C if:
    /// - C is derived from the @ref ecstasy::Resource class and match the @ref Queryable concept.
    /// - C match the @ref ecstasy::IsStorage concept and match the @ref Queryable concept.
    /// - C is a const type matching the @ref ConstQueryableObject concept.
    /// - C match the @ref ecstasy::query::Modifier
    /// Returns C::Modifier if C match the @ref ecstasy::RegistryModifier concept.
    /// Otherwise, returns @ref getStorageType<C>.
    ///
    /// @tparam C Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename C>
    struct queryable_type {
        using type = getStorageType<C>;
    };

    /// @copydoc queryable_type
    template <query::Queryable Q>
        requires query::ConstQueryableObject<Q>
    struct queryable_type<const Q> {
        using type = const queryable_type<Q>::type;
    };

    /// @copydoc queryable_type
        requires query::Queryable<R>
    struct queryable_type<R> {
        using type = R;
    };

    /// @copydoc queryable_type
    template <IsStorage S>
        requires query::Queryable<S>
    struct queryable_type<S> {
        using type = S;
    };

    /// @copydoc queryable_type
    template <ecstasy::query::Modifier M>
    struct queryable_type<M> {
        using type = M;
    };

    /// @copydoc queryable_type
    template <RegistryModifier M>
    struct queryable_type<M> {
        using type = typename M::Modifier;
    };

    ///
    /// @brief Helper to @ref queryable_type<C>::type.
    ///
    /// @tparam C Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename C>
    using queryable_type_t = typename queryable_type<C>::type;
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_CONCEPTS_QUERYABLETYPE_HPP_ */
