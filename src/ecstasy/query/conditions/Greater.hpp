///
/// @file Greater.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-16
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONDITIONS_GREATER_HPP_
#define ECSTASY_QUERY_CONDITIONS_GREATER_HPP_

#include "Condition.hpp"

namespace ecstasy
{
    /// @brief Greater than condition.
    template <auto Left, auto Right>
    using Greater = query::Condition<Left, Right, std::greater<>>;
} // namespace ecstasy

#endif /* !ECSTASY_QUERY_CONDITIONS_GREATER_HPP_ */
