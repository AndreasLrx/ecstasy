///
/// @file util.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_UTIL_HPP_
#define ECSTASY_QUERY_CONCEPTS_UTIL_HPP_

#include <concepts>
#include <cstddef>

namespace ecstasy::query
{
    ///
    /// @brief Tests if the type @p T exists in the types @p Ts.
    ///
    /// @tparam T Searched type.
    /// @tparam Ts @a Valid types.
    ///
    /// @return constexpr bool Whether the type @p T is contained in the types @p Ts.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename T, typename... Ts>
    constexpr bool contains()
    {
        return std::disjunction_v<std::is_same<T, Ts>...>;
    }

    ///
    /// @brief Fetch the type of a parameter pack types from its index.
    ///
    /// @note The @b type using is the @p Index'th types in @p Types.
    ///
    /// @tparam Index Index of the type to fetch.
    /// @tparam Types Types to search in.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <size_t Index, typename... Types>
    struct type_at_index {
        using type = std::tuple_element_t<Index, std::tuple<Types...>>;
    };

    ///
    /// @brief Fetch the type of a parameter pack types from its index.
    ///
    /// @note The result is the @p Index'th types in @p Types.
    ///
    /// @tparam Index Index of the type to fetch.
    /// @tparam Types Types to search in.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <size_t Index, typename... Types>
    using type_at_index_t = typename type_at_index<Index, Types...>::type;

    ///
    /// @brief Extract the first type from @p Types.
    ///
    /// @tparam Types Types to search in.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename... Types>
    using first_type_t = type_at_index_t<0, Types...>;

    /// @internal
    /// @brief Helper for @ref type_set_eq.
    ///
    /// @tparam typename First types set.
    /// @tparam typename Second types set.
    /// @tparam typename Internal type used for specialization.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename, typename, typename = void>
    struct type_set_eq_helper : public std::true_type {
    };

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
        struct type_set_eq_helper < std::tuple<T1, Ts1...>,
        std::tuple<Ts2...>,
        typename std::enable_if<
            !contains<T1, Ts2...>() || 
            !type_set_eq_helper<std::tuple<Ts1...>, std::tuple<Ts2...>>::value
            >::type> : public std::false_type {
    };

    ///
    /// @brief Test if two tuple types contains the exact same type independently of the order.
    /// 
    /// @tparam typename Firs type set (wrapped in a tuple).
    /// @tparam typename Second type set (wrapped in a tuple type).
    /// @tparam typename Internal type used for specialization.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename, typename, typename = void>
    struct type_set_eq : public std::true_type {
    };

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
    struct type_set_eq<std::tuple<Ts1...>, std::tuple<Ts2...>,
        typename std::enable_if<
            (sizeof...(Ts1) != sizeof...(Ts2)) || 
            !type_set_eq_helper<std::tuple<Ts1...>, std::tuple<Ts2...>>::value>
            ::type> : std::false_type {
    };

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
    inline bool constexpr type_set_eq_v = true;

    ///
    /// @brief Helper for @ref type_set_eq.
    /// 
    /// @tparam Ts1 First type evaluated.
    /// @tparam Ts2 Second type evaluated.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename... Ts1, typename... Ts2>
    inline bool constexpr type_set_eq_v<std::tuple<Ts1...>, std::tuple<Ts2...>> = 
        type_set_eq<std::tuple<Ts1...>, std::tuple<Ts2...>>::value;

    // clang-format on
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_UTIL_HPP_ */
