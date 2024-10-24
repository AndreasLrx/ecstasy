///
/// @file Less.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Less than condition.
/// @version 1.0.0
/// @date 2022-12-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONDITIONS_LESS_HPP_
#define ECSTASY_QUERY_CONDITIONS_LESS_HPP_

#include "Condition.hpp"

namespace ecstasy
{
    /// @brief Less than condition.
    template <auto Left, auto Right>
    using Less = query::Condition<Left, Right, std::less<>>;
} // namespace ecstasy

#endif /* !ECSTASY_QUERY_CONDITIONS_LESS_HPP_ */
