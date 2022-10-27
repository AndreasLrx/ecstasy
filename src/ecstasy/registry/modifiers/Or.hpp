///
/// @file Or.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_MODIFIERS_OR_HPP_
#define ECSTASY_REGISTRY_MODIFIERS_OR_HPP_

#include "ecstasy/query/modifiers/Or.hpp"

namespace ecstasy
{
    ///
    /// @brief Placeholder for @ref ecstasy::query::modifier::Or.
    ///
    /// @tparam C1 left operand type.
    /// @tparam C2 right operand type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C1, typename C2>
    struct Or {
        using Modifier = query::modifier::Or<getStorageType<C1>, getStorageType<C2>>;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_MODIFIERS_OR_HPP_ */
