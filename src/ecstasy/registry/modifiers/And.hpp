///
/// @file And.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Binary query modifier which performs a and between at least two queryables.
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_MODIFIERS_AND_HPP_
#define ECSTASY_REGISTRY_MODIFIERS_AND_HPP_

#include "ecstasy/config.hpp"
#include "ecstasy/query/modifiers/And.hpp"

namespace ecstasy
{
    ///
    /// @brief Placeholder for @ref ecstasy::query::modifier::And "And".
    ///
    /// @tparam C1 left operand type.
    /// @tparam C2 right operand type.
    /// @tparam Cs additional operand types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C1, typename C2, typename... Cs>
    struct And {
        using Modifier = query::modifier::AndImpl<thread::AUTO_LOCK_DEFAULT, queryable_type_t<C1>, queryable_type_t<C2>,
            queryable_type_t<Cs>...>;
    };

    ///
    /// @brief Placeholder for @ref ecstasy::query::modifier::And "And". This extended version allows to specify the
    /// AutoLock param.
    ///
    /// @tparam AutoLock Whether the queryables should be locked automatically.
    /// @tparam C1 left operand type.
    /// @tparam C2 right operand type.
    /// @tparam Cs additional operand types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-07)
    ///
    template <bool AutoLock, typename C1, typename C2, typename... Cs>
    struct AndEx {
        using Modifier =
            query::modifier::AndImpl<AutoLock, queryable_type_t<C1>, queryable_type_t<C2>, queryable_type_t<Cs>...>;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_MODIFIERS_AND_HPP_ */
