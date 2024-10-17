///
/// @file modifier_allocator_size.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Get the allocator required size for a list of queryables.
/// @version 1.0.0
/// @date 2023-11-08
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_CONCEPTS_MODIFIERALLOCATORSIZE_HPP_
#define ECSTASY_REGISTRY_CONCEPTS_MODIFIERALLOCATORSIZE_HPP_

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
    struct modifiers_allocator_size : public std::integral_constant<size_t, 0> {};

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
    struct modifier_allocator_size : public std::integral_constant<size_t, 0> {};

    /// @copydoc modifier_allocator_size
    template <RegistryModifier M>
    struct modifier_allocator_size<M> : public modifier_allocator_size<typename M::Modifier> {};

    /// @copydoc modifier_allocator_size
    template <query::Modifier M>
    struct modifier_allocator_size<M>
        : public std::integral_constant<size_t, sizeof(M) + modifier_allocator_size<typename M::Operands>::value> {};

    /// @copydoc modifier_allocator_size
    template <typename Q, typename... Qs>
    struct modifier_allocator_size<util::meta::Traits<Q, Qs...>>
        : public std::integral_constant<size_t, modifiers_allocator_size<Q, Qs...>::value> {};

    ///
    /// @brief Helper for modifier_allocator_size<Q>::value.
    ///
    /// @tparam Q Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2023-11-08)
    ///
    template <typename Q>
    size_t constexpr modifier_allocator_size_v = modifier_allocator_size<Q>::value;

    /// @copydoc modifiers_allocator_size
    template <typename Q, typename... Qs>
    struct modifiers_allocator_size<Q, Qs...>
        : public std::integral_constant<size_t, modifier_allocator_size_v<Q> + modifiers_allocator_size<Qs...>::value> {
    };

    ///
    /// @brief Helper for modifiers_allocator_size<Q, Qs...>::value.
    ///
    /// @tparam Q Evaluated type.
    /// @tparam Qs Next evaluated types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2023-11-08)
    ///
    template <typename Q, typename... Qs>
    size_t constexpr modifiers_allocator_size_v = modifiers_allocator_size<Q, Qs...>::value;

} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_CONCEPTS_MODIFIERALLOCATORSIZE_HPP_ */
