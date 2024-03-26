///
/// @file GreaterEqual.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONDITIONS_GREATEREQUAL_HPP_
#define ECSTASY_QUERY_CONDITIONS_GREATEREQUAL_HPP_

#include "Condition.hpp"

namespace ecstasy
{
    /// @brief Greater than or equal condition.
    template <auto Left, auto Right>
    using GreaterEqual = query::Condition<Left, Right, std::greater_equal<>>;
} // namespace ecstasy

#endif /* !ECSTASY_QUERY_CONDITIONS_GREATEREQUAL_HPP_ */
