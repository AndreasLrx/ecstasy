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
    struct ConditionBase {};

    template <auto Left, auto Right, typename Comparer>
    struct Condition : public ConditionBase {};

    /// @note Const - Const, Will therefore always output the same result
    // clang-format off
    template <
        typename LeftType, LeftType Left, 
        typename RightType, RightType Right, 
        typename Comparer>
    requires(!std::is_member_pointer_v<LeftType> && !std::is_member_pointer_v<RightType>) 
    struct Condition<Left, Right, Comparer> : public ConditionBase {
        //clang-format on
        static bool test()
        {
            return Comparer{}(Left, Right);
        }
    };

    /// @note Const - Member
    // clang-format off
    template <
        typename LeftType, LeftType Left, 
        typename RightType, typename RightValueType, RightValueType RightType::*ptr, 
        typename Comparer>
    requires(!std::is_member_pointer_v<LeftType> && std::is_member_object_pointer_v<decltype(ptr)>) 
        struct Condition<Left, ptr, Comparer> : public ConditionBase {
        //clang-format on
        using Right = RightType;

        static bool test(const RightType &right)
        {
            return Comparer{}(Left, right.*ptr);
        }
    };

    /// @note Const - Getter
    // clang-format off
    template <
        typename LeftType, LeftType Left, 
        typename RightType, typename RightValueType, RightValueType (RightType::*ptr)(void) const, 
        typename Comparer>
    requires(!std::is_member_pointer_v<LeftType> && std::is_member_function_pointer_v<decltype(ptr)>) 
        struct Condition<Left, ptr, Comparer> : public ConditionBase {
        // clang-format on
        using Right = RightType;

        static bool test(const RightType &right)
        {
            return Comparer{}(Left, (right.*ptr)());
        }
    };

    /// @note Member - Const
    // clang-format off
    template <
        typename LeftType, typename LeftValueType, LeftValueType LeftType::*ptr, 
        typename RightType, RightType Right, 
        typename Comparer>
    requires(std::is_member_object_pointer_v<decltype(ptr)> && !std::is_member_pointer_v<RightType>) 
    struct Condition<ptr, Right, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;

        static bool test(const LeftType &left)
        {
            return Comparer{}(left.*ptr, Right);
        }
    };

    /// @note Member - Member
    // clang-format off
    template <
        typename LeftType, typename LeftValueType, LeftValueType LeftType::*ptrLeft, 
        typename RightType, typename RightValueType, RightValueType RightType::*ptrRight, 
        typename Comparer>
    requires(std::is_member_object_pointer_v<decltype(ptrLeft)> && std::is_member_object_pointer_v<decltype(ptrRight)>) 
    struct Condition<ptrLeft, ptrRight, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;
        using Right = RightType;

        static bool test(const LeftType &left, const RightType &right)
        {
            return Comparer{}(left.*ptrLeft, right.*ptrRight);
        }
    };

    /// @note Member - Getter
    // clang-format off
    template <
        typename LeftType, typename LeftValueType, LeftValueType LeftType::*ptrLeft, 
        typename RightType, typename RightValueType, RightValueType (RightType::*ptrRight)(void) const, 
        typename Comparer>
    requires(std::is_member_object_pointer_v<decltype(ptrLeft)> && std::is_member_function_pointer_v<decltype(ptrRight)>) 
    struct Condition<ptrLeft, ptrRight, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;
        using Right = RightType;

        static bool test(const LeftType &left, const RightType &right)
        {
            return Comparer{}(left.*ptrLeft, (right.*ptrRight)());
        }
    };

    /// @note Getter - Const
    // clang-format off
    template <
        typename LeftType, typename LeftValueType, LeftValueType (LeftType::*ptr)(void) const, 
        typename RightType, RightType Right, 
        typename Comparer>
    requires(std::is_member_function_pointer_v<decltype(ptr)> && !std::is_member_pointer_v<RightType>) 
    struct Condition<ptr, Right, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;

        static bool test(const LeftType &left)
        {
            return Comparer{}((left.*ptr)(), Right);
        }
    };

    /// @note Getter - Member
    // clang-format off
    template <
        typename LeftType, typename LeftValueType, LeftValueType (LeftType::*ptrLeft)(void) const, 
        typename RightType, typename RightValueType, RightValueType RightType::*ptrRight, 
        typename Comparer>
    requires(std::is_member_function_pointer_v<decltype(ptrLeft)> && std::is_member_object_pointer_v<decltype(ptrRight)>) 
    struct Condition<ptrLeft, ptrRight, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;
        using Right = RightType;

        static bool test(const LeftType &left, const RightType &right)
        {
            return Comparer{}((left.*ptrLeft)(), right.*ptrRight);
        }
    };

    /// @note Getter - Getter
    // clang-format off
    template <
        typename LeftType, typename LeftValueType, LeftValueType (LeftType::*ptrLeft)(void) const, 
        typename RightType, typename RightValueType, RightValueType (RightType::*ptrRight)(void) const, 
        typename Comparer>
    requires(std::is_member_function_pointer_v<decltype(ptrLeft)> && std::is_member_function_pointer_v<decltype(ptrRight)>) 
    struct Condition<ptrLeft, ptrRight, Comparer> : public ConditionBase {
        // clang-format on
        using Left = LeftType;
        using Right = RightType;

        static bool test(const LeftType &left, const RightType &right)
        {
            return Comparer{}((left.*ptrLeft)(), (right.*ptrRight)());
        }
    };
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONDITIONS_CONDITION_HPP_ */
