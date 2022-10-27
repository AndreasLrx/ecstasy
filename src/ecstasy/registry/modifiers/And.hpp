///
/// @file And.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_MODIFIERS_AND_HPP_
#define ECSTASY_REGISTRY_MODIFIERS_AND_HPP_

#include "ecstasy/query/modifiers/And.hpp"

namespace ecstasy
{
    ///
    /// @brief Placeholder for @ref ecstasy::query::modifier::And.
    ///
    /// @tparam C1 left operand type.
    /// @tparam C2 right operand type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C1, typename C2>
    struct And {
        using Modifier = query::modifier::And<getStorageType<C1>, getStorageType<C2>>;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_MODIFIERS_AND_HPP_ */
