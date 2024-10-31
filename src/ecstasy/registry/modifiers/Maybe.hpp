///
/// @file Maybe.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Query modifier which returns a std::optional filled when the data if existing.
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_MODIFIERS_MAYBE_HPP_
#define ECSTASY_REGISTRY_MODIFIERS_MAYBE_HPP_

#include "ecstasy/config.hpp"
#include "ecstasy/query/modifiers/Maybe.hpp"
#include "ecstasy/registry/concepts/queryable_type.hpp"

namespace ecstasy
{
    ///
    /// @brief Placeholder for @ref ecstasy::query::modifier::Maybe "Maybe".
    ///
    /// @tparam C Component type.
    /// @tparam AutoLock Whether the queryable should be locked automatically.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C, bool AutoLock = thread::AUTO_LOCK_DEFAULT>
    struct Maybe {
        using Modifier = query::modifier::Maybe<queryable_type_t<C>, AutoLock>;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_MODIFIERS_MAYBE_HPP_ */
