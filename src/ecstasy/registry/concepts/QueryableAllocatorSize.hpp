///
/// @file QueryableAllocatorSize.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2023-11-08
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_CONCEPTS_QUERYABLEALLOCATORSIZE_HPP_
#define ECSTASY_REGISTRY_CONCEPTS_QUERYABLEALLOCATORSIZE_HPP_

#include "ecstasy/query/concepts/Modifier.hpp"
#include "ecstasy/query/concepts/Queryable.hpp"
#include "ecstasy/registry/concepts/RegistryModifier.hpp"

namespace ecstasy
{
    ///
    /// @brief Get the allocator required size for a list of queryables. It is equal to the sum of the modifiers memory
    /// size (if any). Returns 0 if there is no modifier.
    ///
    /// @tparam Qs Evaluated types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2023-11-08)
    ///
    template <typename... Qs>
    struct queryables_allocator_size : public std::integral_constant<size_t, 0> {};

    ///
    /// @brief Get the allocator required size for this queryable. It is equal to the modifier memory size (if any).
    /// Returns 0 if there is no modifier.
    ///
    /// @tparam Q Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2023-11-08)
    ///
    template <typename Q>
    struct queryable_allocator_size : public std::integral_constant<size_t, 0> {};

    /// @copydoc queryable_allocator_size
    template <RegistryModifier M>
    struct queryable_allocator_size<M> : public queryable_allocator_size<typename M::Modifier> {};

    /// @copydoc queryable_allocator_size
    template <query::Modifier M>
    struct queryable_allocator_size<M>
        : public std::integral_constant<size_t, sizeof(M) + queryable_allocator_size<typename M::Operands>::value> {};

    /// @copydoc queryable_allocator_size
    template <typename Q, typename... Qs>
    struct queryable_allocator_size<std::tuple<Q, Qs...>>
        : public std::integral_constant<size_t, queryables_allocator_size<Q, Qs...>::value> {};

    ///
    /// @brief Helper for queryable_allocator_size<Q>::value.
    ///
    /// @tparam Q Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2023-11-08)
    ///
    template <typename Q>
    size_t constexpr queryable_allocator_size_v = queryable_allocator_size<Q>::value;

    /// @copydoc queryables_allocator_size
    template <typename Q, typename... Qs>
    struct queryables_allocator_size<Q, Qs...>
        : public std::integral_constant<size_t,
              queryable_allocator_size_v<Q> + queryables_allocator_size<Qs...>::value> {};

    ///
    /// @brief Helper for queryables_allocator_size<Q, Qs...>::value.
    ///
    /// @tparam Q Evaluated type.
    /// @tparam Qs Next evaluated types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2023-11-08)
    ///
    template <typename Q, typename... Qs>
    size_t constexpr queryables_allocator_size_v = queryables_allocator_size<Q, Qs...>::value;

} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_CONCEPTS_QUERYABLEALLOCATORSIZE_HPP_ */
