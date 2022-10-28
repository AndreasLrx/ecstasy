///
/// @file Traits.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-28
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_META_TRAITS_HPP_
#define UTIL_META_TRAITS_HPP_

#include <tuple>

namespace util::meta
{
    ///
    /// @brief Empty parameter pack helper type.
    ///
    /// @tparam Args Empty parameter pack types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename... Args>
    struct Traits {
        /// Get a tuple type containing the types.
        using Tuple = std::tuple<Args...>;
        /// Number of types.
        static constexpr auto Size = sizeof...(Args);
        /// Get the type at the index N.
        template <std::size_t N>
        using Nth = typename std::tuple_element<N, Tuple>::type;
    };

    ///
    /// @brief Parameter pack helper type.
    ///
    /// @tparam Arg First parameter pack type.
    /// @tparam Args Parameter pack types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-28)
    ///
    template <typename Arg, typename... Args>
    struct Traits<Arg, Args...> {
        /// Get a tuple type containing the types.
        using Tuple = std::tuple<Arg, Args...>;
        /// Number of types.
        static constexpr size_t Size = 1 + sizeof...(Args);
        /// Get the type at the index N.
        template <std::size_t N>
        using Nth = typename std::tuple_element<N, Tuple>::type;
        /// Get the first type.
        using First = Nth<0>;
        /// Get the last type.
        using Last = Nth<Size - 1>;
    };

} // namespace util::meta

#endif /* !UTIL_META_TRAITS_HPP_ */
