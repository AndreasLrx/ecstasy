///
/// @file Not.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_MODIFIERS_NOT_HPP_
#define ECSTASY_REGISTRY_MODIFIERS_NOT_HPP_

#include "ecstasy/query/modifiers/Not.hpp"

namespace ecstasy
{
    ///
    /// @brief Placeholder for @ref ecstasy::query::modifier::Not.
    ///
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C>
    struct Not {
        using Modifier = query::modifier::Not<getStorageType<C>>;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_MODIFIERS_NOT_HPP_ */
