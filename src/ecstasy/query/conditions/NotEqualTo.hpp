///
/// @file NotEqualTo.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONDITIONS_NOTEQUALTO_HPP_
#define ECSTASY_QUERY_CONDITIONS_NOTEQUALTO_HPP_

#include "Condition.hpp"

namespace ecstasy
{
    /// @brief Inequality condition.
    template <auto Left, auto Right>
    using NotEqualTo = query::Condition<Left, Right, std::not_equal_to<>>;
} // namespace ecstasy

#endif /* !ECSTASY_QUERY_CONDITIONS_NOTEQUALTO_HPP_ */
