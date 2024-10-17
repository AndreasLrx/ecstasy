///
/// @file Not.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Query modifier which returns a std::optional filled when the data if existing.
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_MODIFIERS_NOT_HPP_
#define ECSTASY_REGISTRY_MODIFIERS_NOT_HPP_

#include "ecstasy/config.hpp"
#include "ecstasy/query/modifiers/Not.hpp"

namespace ecstasy
{
    ///
    /// @brief Placeholder for @ref ecstasy::query::modifier::Not.
    ///
    /// @tparam C Component type.
    /// @tparam AutoLock Whether the queryable should be locked automatically.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename C, bool AutoLock = thread::AUTO_LOCK_DEFAULT>
    struct Not {
        using Modifier = query::modifier::Not<queryable_type_t<C>, AutoLock>;
    };
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_MODIFIERS_NOT_HPP_ */
