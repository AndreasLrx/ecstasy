///
/// @file Condition.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Contains all the condition concepts.
/// @version 1.0.0
/// @date 2022-12-16
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_CONDITION_HPP_
#define ECSTASY_QUERY_CONCEPTS_CONDITION_HPP_

#include "ecstasy/query/conditions/Condition.hpp"

namespace ecstasy::query
{
    ///
    /// @brief Query Condition concept.
    /// You should use @ref ecstasy::query::QConditionConst "QConditionConst",
    /// @ref ecstasy::query::QConditionLeft "QConditionLeft", @ref ecstasy::query::QConditionRight "QConditionRight" or
    /// @ref ecstasy::query::QConditionLeftRight "QConditionLeftRight" to be sure that the condition is valid.
    ///
    /// @warning
    /// This concept isn't sufficient to check if a type is a condition, it only checks if it derives from
    /// ConditionBase.
    ///
    /// @tparam C The type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-16)
    ///
    template <typename C>
    concept QCondition = requires(C &condition) { requires std::derived_from<C, ecstasy::query::ConditionBase>; };

    ///
    /// @brief Condition with two constant operands. Meaning that the condition should always output the same result.
    /// @see QCondition
    ///
    /// @tparam C The type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-16)
    ///
    template <typename C>
    concept QConditionConst = requires(C &condition) {
        requires QCondition<C>;

        // clang-format off
        { C::test() } -> std::same_as<bool>;
        // clang-format on
    };

    ///
    /// @brief Condition with a runtime left operand and a constant right operand.
    /// @see QCondition
    ///
    /// @tparam C The type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-16)
    ///
    template <typename C>
    concept QConditionLeft = requires(C &condition, const typename C::Left &left) {
        requires QCondition<C>;

        typename C::Left;

        // clang-format off
        { C::test(left) } -> std::same_as<bool>;
        // clang-format on
    };

    ///
    /// @brief Condition with a constant left operand and a runtime right operand.
    /// @see QCondition
    ///
    /// @tparam C The type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-16)
    ///
    template <typename C>
    concept QConditionRight = requires(C &condition, const typename C::Right &right) {
        requires QCondition<C>;

        typename C::Right;

        // clang-format off
        { C::test(right) } -> std::same_as<bool>;
        // clang-format on
    };

    ///
    /// @brief Condition with two runtime operands.
    /// @see QCondition
    ///
    /// @tparam C The type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-12-16)
    ///
    template <typename C>
    concept QConditionLeftRight = requires(C &condition, const typename C::Left &left, const typename C::Right &right) {
        requires QCondition<C>;

        typename C::Left;
        typename C::Right;

        // clang-format off
        { C::test(left, right) } -> std::same_as<bool>;
        // clang-format on
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_CONDITION_HPP_ */
