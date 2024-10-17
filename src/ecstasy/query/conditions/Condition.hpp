///
/// @file Condition.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Contains the condition resolution structures.
/// @version 1.0.0
/// @date 2022-12-15
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_QUERY_CONDITIONS_CONDITION_HPP_
#define ECSTASY_QUERY_CONDITIONS_CONDITION_HPP_

#include <concepts>

namespace ecstasy::query
{
    ///
    /// @brief Base class for all conditions.
    /// This is required to store the conditions in a container with type erasure.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    struct ConditionBase {};

    ///
    /// @brief Condition resolution structure.
    /// The operands can be one of those (either left or right operand):
    /// - Const value: Condition<5, 5, Comparer>
    /// - Member pointer: Condition<&Type::value, 5, Comparer>
    /// - Getter pointer: Condition<&Type::getValue, 5, Comparer>
    ///
    /// @tparam Left Left operand.
    /// @tparam Right Right operand.
    /// @tparam Comparer Comparer to use.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <auto Left, auto Right, typename Comparer>
    struct Condition : public ConditionBase {};

    // clang-format off
    ///
    /// @brief Condition resolution structure for two constant operands.
    ///
    /// @tparam LeftType Left operand type.
    /// @tparam Left Left operand value (const).
    /// @tparam RightType Right operand type.
    /// @tparam Right Right operand value (const).
    /// @tparam Comparer Comparer to use. 
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, LeftType Left, 
        typename RightType, RightType Right, 
        typename Comparer>
    requires(!std::is_member_pointer_v<LeftType> && !std::is_member_pointer_v<RightType>) 
    struct Condition<Left, Right, Comparer> : public ConditionBase {
        //clang-format on

        ///
        /// @brief Evaluate the condition.
        /// 
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test()
        {
            return Comparer{}(Left, Right);
        }
    };

    
    // clang-format off
    ///
    /// @brief Condition resolution structure for a constant left operand and a member right operand.
    /// 
    /// @tparam LeftType Left operand type.
    /// @tparam Left Left operand value (const).
    /// @tparam RightType Class containing the right operand.
    /// @tparam RightValueType Right operand type.
    /// @tparam RightType::*ptr Member pointer to the right operand.
    /// @tparam Comparer Comparer to use. 
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, LeftType Left, 
        typename RightType, typename RightValueType, RightValueType RightType::*ptr, 
        typename Comparer>
    requires(!std::is_member_pointer_v<LeftType> && std::is_member_object_pointer_v<decltype(ptr)>) 
        struct Condition<Left, ptr, Comparer> : public ConditionBase {
        //clang-format on
        using Right = RightType;

        ///
        /// @brief Evaluate the condition.
        /// 
        /// @param[in] right Right operand class instance.
        ///
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test(const RightType &right)
        {
            return Comparer{}(Left, right.*ptr);
        }
    };

    // clang-format off
    ///
    /// @brief Condition resolution structure for a constant left operand and a getter right operand.
    /// 
    /// @tparam LeftType Left operand type.
    /// @tparam Left Left operand value (const).
    /// @tparam RightType Class containing the right operand.
    /// @tparam RightValueType Right operand type.
    /// @tparam ptr Getter pointer to the right operand.
    /// @tparam Comparer Comparer to use.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, LeftType Left, 
        typename RightType, typename RightValueType, RightValueType (RightType::*ptr)(void) const, 
        typename Comparer>
    requires(!std::is_member_pointer_v<LeftType> && std::is_member_function_pointer_v<decltype(ptr)>) 
        struct Condition<Left, ptr, Comparer> : public ConditionBase {
        // clang-format on
        using Right = RightType;

        ///
        /// @brief Evaluate the condition.
        ///
        /// @param[in] right Right operand class instance.
        ///
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test(const RightType &right)
        {
            return Comparer{}(Left, (right.*ptr)());
        }
    };

    // clang-format off
    ///
    /// @brief Condition resolution structure for a member left operand and a constant right operand.
    /// 
    /// @tparam LeftType Class containing the left operand.
    /// @tparam LeftValueType Left operand type.
    /// @tparam LeftType::*ptr Member pointer to the left operand.
    /// @tparam RightType Right operand type.
    /// @tparam Right Right operand value (const).
    /// @tparam Comparer Comparer to use.
    /// 
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, typename LeftValueType, LeftValueType LeftType::*ptr, 
        typename RightType, RightType Right, 
        typename Comparer>
    requires(std::is_member_object_pointer_v<decltype(ptr)> && !std::is_member_pointer_v<RightType>) 
    struct Condition<ptr, Right, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;

        ///
        /// @brief Evaluate the condition.
        ///
        /// @param[in] left Left operand class instance.
        ///
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test(const LeftType &left)
        {
            return Comparer{}(left.*ptr, Right);
        }
    };

    // clang-format off
    ///
    /// @brief Condition resolution structure for a member left operand and a member right operand.
    /// 
    /// @tparam LeftType Class containing the left operand.
    /// @tparam LeftValueType Left operand type.
    /// @tparam LeftType::*ptrLeft Member pointer to the left operand.
    /// @tparam RightType Class containing the right operand.
    /// @tparam RightValueType Right operand type.
    /// @tparam RightType::*ptrRight Member pointer to the right operand.
    /// @tparam Comparer Comparer to use.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, typename LeftValueType, LeftValueType LeftType::*ptrLeft, 
        typename RightType, typename RightValueType, RightValueType RightType::*ptrRight, 
        typename Comparer>
    requires(std::is_member_object_pointer_v<decltype(ptrLeft)> && std::is_member_object_pointer_v<decltype(ptrRight)>) 
    struct Condition<ptrLeft, ptrRight, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;
        using Right = RightType;

        ///
        /// @brief Evaluate the condition.
        ///
        /// @param[in] left Left operand class instance.
        /// @param[in] right Right operand class instance.
        ///
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test(const LeftType &left, const RightType &right)
        {
            return Comparer{}(left.*ptrLeft, right.*ptrRight);
        }
    };

    // clang-format off
    ///
    /// @brief Condition resolution structure for a member left operand and a getter right operand.
    /// 
    /// @tparam LeftType Class containing the left operand.
    /// @tparam LeftValueType Left operand type.
    /// @tparam LeftType::*ptrLeft Member pointer to the left operand.
    /// @tparam RightType Class containing the right operand.
    /// @tparam RightValueType Right operand type.
    /// @tparam ptr Getter pointer to the right operand getter.
    /// @tparam Comparer Comparer to use.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, typename LeftValueType, LeftValueType LeftType::*ptrLeft, 
        typename RightType, typename RightValueType, RightValueType (RightType::*ptrRight)(void) const, 
        typename Comparer>
    requires(std::is_member_object_pointer_v<decltype(ptrLeft)> && std::is_member_function_pointer_v<decltype(ptrRight)>) 
    struct Condition<ptrLeft, ptrRight, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;
        using Right = RightType;

        ///
        /// @brief Evaluate the condition.
        ///
        /// @param[in] left Left operand class instance.
        /// @param[in] right Right operand class instance.
        ///
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test(const LeftType &left, const RightType &right)
        {
            return Comparer{}(left.*ptrLeft, (right.*ptrRight)());
        }
    };

    // clang-format off
    ///
    /// @brief Condition resolution structure for a getter left operand and a constant right operand.
    /// 
    /// @tparam LeftType Class containing the left operand.
    /// @tparam LeftValueType Left operand type.
    /// @tparam ptr Getter pointer to the left operand getter.
    /// @tparam RightType Right operand type.
    /// @tparam Right Right operand value (const).
    /// @tparam Comparer Comparer to use.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, typename LeftValueType, LeftValueType (LeftType::*ptr)(void) const, 
        typename RightType, RightType Right, 
        typename Comparer>
    requires(std::is_member_function_pointer_v<decltype(ptr)> && !std::is_member_pointer_v<RightType>) 
    struct Condition<ptr, Right, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;

        ///
        /// @brief Evaluate the condition.
        ///
        /// @param[in] left Left operand class instance.
        ///
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test(const LeftType &left)
        {
            return Comparer{}((left.*ptr)(), Right);
        }
    };

    // clang-format off
    ///
    /// @brief Condition resolution structure for a getter left operand and a member right operand.
    /// 
    /// @tparam LeftType Class containing the left operand.
    /// @tparam LeftValueType Left operand type.
    /// @tparam ptr Getter pointer to the left operand getter.
    /// @tparam RightType Class containing the right operand.
    /// @tparam RightValueType Right operand type.
    /// @tparam RightType::*ptrRight Member pointer to the right operand.
    /// @tparam Comparer Comparer to use.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, typename LeftValueType, LeftValueType (LeftType::*ptrLeft)(void) const, 
        typename RightType, typename RightValueType, RightValueType RightType::*ptrRight, 
        typename Comparer>
    requires(std::is_member_function_pointer_v<decltype(ptrLeft)> && std::is_member_object_pointer_v<decltype(ptrRight)>) 
    struct Condition<ptrLeft, ptrRight, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;
        using Right = RightType;

        ///
        /// @brief Evaluate the condition.
        ///
        /// @param[in] left Left operand class instance.
        /// @param[in] right Right operand class instance.
        ///
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test(const LeftType &left, const RightType &right)
        {
            return Comparer{}((left.*ptrLeft)(), right.*ptrRight);
        }
    };

    // clang-format off
    ///
    /// @brief Condition resolution structure for two getter operands.
    /// 
    /// @tparam LeftType Class containing the left operand.
    /// @tparam LeftValueType Left operand type.
    /// @tparam ptr Getter pointer to the left operand getter.
    /// @tparam RightType Class containing the right operand.
    /// @tparam RightValueType Right operand type.
    /// @tparam ptr Getter pointer to the right operand getter.
    /// @tparam Comparer Comparer to use.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-17)
    ///
    template <
        typename LeftType, typename LeftValueType, LeftValueType (LeftType::*ptrLeft)(void) const, 
        typename RightType, typename RightValueType, RightValueType (RightType::*ptrRight)(void) const, 
        typename Comparer>
    requires(std::is_member_function_pointer_v<decltype(ptrLeft)> && std::is_member_function_pointer_v<decltype(ptrRight)>) 
    struct Condition<ptrLeft, ptrRight, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;
        using Right = RightType;

        ///
        /// @brief Evaluate the condition.
        ///
        /// @param[in] left Left operand class instance.
        /// @param[in] right Right operand class instance.
        ///
        /// @return bool True if the condition is met.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-10-17)
        ///
        static bool test(const LeftType &left, const RightType &right)
        {
            return Comparer{}((left.*ptrLeft)(), (right.*ptrRight)());
        }
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONDITIONS_CONDITION_HPP_ */
