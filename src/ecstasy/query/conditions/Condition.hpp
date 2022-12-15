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

    template <typename LeftType, typename LeftValueType, LeftValueType LeftType::*ptr, typename RightType,
        RightType Right, typename Comparer>
    requires(!std::is_function_v<LeftValueType>) struct Condition<ptr, Right, Comparer> {
        using Left = LeftType;

        static bool test(const LeftType &left)
        {
            return Comparer{}(left.*ptr, Right);
        }
    };

    template <typename LeftType, typename LeftValueType, LeftValueType (LeftType::*ptr)(void) const, typename RightType,
        RightType Right, typename Comparer>
    struct Condition<ptr, Right, Comparer> {
        static bool test(const LeftType &left)
        {
            return Comparer{}((left.*ptr)(), Right);
        }
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONDITIONS_CONDITION_HPP_ */
