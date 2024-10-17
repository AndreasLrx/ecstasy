///
/// @file EqualTo.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Equality condition.
/// @version 1.0.0
/// @date 2022-12-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONDITIONS_EQUALTO_HPP_
#define ECSTASY_QUERY_CONDITIONS_EQUALTO_HPP_

#include "Condition.hpp"

namespace ecstasy
{
    /// @brief Equality condition.
    template <auto Left, auto Right>
    using EqualTo = query::Condition<Left, Right, std::equal_to<>>;
} // namespace ecstasy

#endif /* !ECSTASY_QUERY_CONDITIONS_EQUALTO_HPP_ */
