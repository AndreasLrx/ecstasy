///
/// @file type_set_eq.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-28
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_META_TYPE_SET_EQ_HPP_
#define UTIL_META_TYPE_SET_EQ_HPP_

#include "contains.hpp"

namespace util::meta
{
    /// @internal
    /// @brief Internal helper for @ref type_set_eq.
    ///
    /// @tparam typename First types set.
    /// @tparam typename Second types set.
    /// @tparam typename Internal type used for specialization.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename, typename>
    struct _type_set_eq : public std::false_type {};

    // clang-format off

    /// @internal
    /// @brief Helper for @ref type_set_eq.
    /// Matches when @b T1 is not contained in @b Ts2 or if @ref type_set_eq_helper<Ts1, Ts2>::value 
    /// is false (ie if one of the following types in @b Ts1 is not contained in @b Ts2).
    /// 
    /// @tparam T1 First type evaluated.
    /// @tparam Ts1 Types to be evaluated.
    /// @tparam Ts2 Second types set.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename T1, typename... Ts1, typename... Ts2>
    struct _type_set_eq<std::tuple<T1, Ts1...>, std::tuple<Ts2...>>: public
        std::conditional_t<
            // If
            util::meta::contains<T1, Ts2...> &&
            ((sizeof...(Ts1) == 0) || _type_set_eq<std::tuple<Ts1...>, std::tuple<Ts2...>>::value),
            // Then
            std::true_type,
            // Else
            std::false_type
        > {
    };

    /// @internal
    /// @brief Helper for @ref type_set_eq.
    /// Specialization for comparison between empty types set
    /// 
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-11-02)
    ///
    template <>
    struct _type_set_eq<std::tuple<>, std::tuple<>> : public std::true_type {
    };

    // clang-format on

    ///
    /// @brief Test if two tuple types contains the exact same type independently of the order.
    ///
    /// @tparam typename Firs type set (wrapped in a tuple).
    /// @tparam typename Second type set (wrapped in a tuple type).
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename, typename>
    struct type_set_eq : public std::false_type {};

    // clang-format off

    /// @internal
    /// @brief Test if two tuple types don't contains the exact same type independently of the order.
    /// Matches when @b Ts1 and @b Ts2 have not the same size or when @b Ts2 does not contains all the types in @b Ts1.
    /// 
    /// @tparam T1 First type evaluated.
    /// @tparam Ts1 Types to be evaluated.
    /// @tparam Ts2 Second types set.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename... Ts1, typename... Ts2>
    struct type_set_eq<std::tuple<Ts1...>, std::tuple<Ts2...>> : public 
        std::conditional_t< 
        // If
        (sizeof...(Ts1) == sizeof...(Ts2)) && 
        _type_set_eq<std::tuple<Ts1...>, std::tuple<Ts2...>>::value,
        // Then
        std::true_type, 
        // Else
        std::false_type > {
    };
    // clang-format on

    ///
    /// @brief Helper for @ref type_set_eq.
    ///
    /// @tparam Ts1 First type evaluated.
    /// @tparam Ts2 Second type evaluated.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename Ts1, typename Ts2>
    inline bool constexpr type_set_eq_v = type_set_eq<Ts1, Ts2>::value;
} // namespace util::meta

#endif /* !UTIL_META_TYPE_SET_EQ_HPP_ */
