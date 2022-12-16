///
/// @file Condition.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-12-16
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_CONDITION_HPP_
#define ECSTASY_QUERY_CONCEPTS_CONDITION_HPP_

#include "ecstasy/query/conditions/Condition.hpp"

namespace ecstasy::query
{
    template <typename C>
    concept QCondition = requires(C &condition)
    {
        requires std::derived_from<C, ecstasy::query::ConditionBase>;
    };

    template <typename C>
    concept QConditionConst = requires(C &condition)
    {
        requires QCondition<C>;

        // clang-format off
        { C::test() } -> std::same_as<bool>;
        // clang-format on
    };

    template <typename C>
    concept QConditionLeft = requires(C &condition, const C::Left &left)
    {
        requires QCondition<C>;

        typename C::Left;

        // clang-format off
        { C::test(left) } -> std::same_as<bool>;
        // clang-format on
    };

    template <typename C>
    concept QConditionRight = requires(C &condition, const C::Right &right)
    {
        requires QCondition<C>;

        typename C::Right;

        // clang-format off
        { C::test(right) } -> std::same_as<bool>;
        // clang-format on
    };

    template <typename C>
    concept QConditionLeftRight = requires(C &condition, const C::Left &left, const C::Right &right)
    {
        requires QCondition<C>;

        typename C::Left;
        typename C::Right;

        // clang-format off
        { C::test(left, right) } -> std::same_as<bool>;
        // clang-format on
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_CONDITION_HPP_ */
