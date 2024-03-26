///
/// @file Maybe.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_MODIFIERS_MAYBE_HPP_
#define ECSTASY_REGISTRY_MODIFIERS_MAYBE_HPP_

#include "ecstasy/query/modifiers/Maybe.hpp"

namespace ecstasy
{
    ///
    /// @brief Placeholder for @ref ecstasy::query::modifier::Maybe.
    ///
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C>
    struct Maybe {
        using Modifier = query::modifier::Maybe<queryable_type_t<C>>;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_MODIFIERS_MAYBE_HPP_ */
