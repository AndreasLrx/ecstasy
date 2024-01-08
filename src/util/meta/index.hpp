///
/// @file find.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-28
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_META_FIND_HPP_
#define UTIL_META_FIND_HPP_

#include <type_traits>

namespace util::meta
{
    ///
    /// @brief Get the index of the first occurence of type @p T in the types @p Ts.
    ///
    /// @warning @p T @b must be contained in @p Ts.
    ///
    /// @tparam T Searched type.
    /// @tparam Ts Types containing @p T.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename T, typename... Ts>
    struct index;

    /// @copydoc index
    template <typename T, typename... Ts>
    struct index<T, T, Ts...> : std::integral_constant<std::size_t, 0> {};

    /// @copydoc index
    template <typename T, typename U, typename... Ts>
    struct index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + index<T, Ts...>::value> {};

    ///
    /// @brief Helper for index<...>::type.
    ///
    /// @tparam T Searched type.
    /// @tparam Ts Types containing @p T.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-29)
    ///
    template <typename T, typename... Ts>
    constexpr inline std::size_t index_v = index<T, Ts...>::value;

} // namespace util::meta

#endif /* !UTIL_META_CONTAINS_HPP_ */
