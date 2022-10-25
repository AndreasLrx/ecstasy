///
/// @file QueryableType.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_CONCEPTS_QUERYABLETYPE_HPP_
#define ECSTASY_REGISTRY_CONCEPTS_QUERYABLETYPE_HPP_

#include "ecstasy/query/concepts/Queryable.hpp"
#include "ecstasy/resource/Resource.hpp"
#include "ecstasy/storage/StorageConcepts.hpp"

namespace ecstasy
{
    ///
    /// @brief Try to get the type of a queryable associated to a type stored in the registry (component, storage,
    /// resource...)
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
    template <std::derived_from<Resource> R>
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

    ///
    /// @brief Helper to @ref queryable_type<C>::type.
    ///
    /// @tparam C Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename C>
    using queryable_type_t = queryable_type<C>::type;
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_CONCEPTS_QUERYABLETYPE_HPP_ */
