///
/// @file Condition.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-15
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONDITIONS_CONDITION_HPP_
#define ECSTASY_QUERY_CONDITIONS_CONDITION_HPP_

#include <concepts>

namespace ecstasy::query
{
    template <auto Left, auto Right, typename Comparer>
    struct Condition {
    };

    template <typename LeftType, typename LeftValueType, LeftValueType LeftType::*Left, typename RightType,
        RightType Right, typename Comparer>
    requires(!std::is_function_v<LeftValueType>) struct Condition<Left, Right, Comparer> {
        Condition() = default;

        bool operator()(const LeftType &left)
        {
            return Comparer{}(left.*Left, Right);
        }
    };

    template <typename LeftType, typename LeftValueType, LeftValueType (LeftType::*Left)(void) const,
        typename RightType, RightType Right, typename Comparer>
    struct Condition<Left, Right, Comparer> {
        Condition() = default;

        bool operator()(const LeftType &left)
        {
            return Comparer{}((left.*Left)(), Right);
        }
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONDITIONS_CONDITION_HPP_ */
