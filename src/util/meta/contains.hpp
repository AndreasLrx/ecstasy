///
/// @file contains.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-28
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef UTIL_META_CONTAINS_HPP_
#define UTIL_META_CONTAINS_HPP_

#include <type_traits>

namespace util::meta
{
    ///
    /// @brief Checks if the type @p T exists in the types @p Ts.
    ///
    /// @tparam T Searched type.
    /// @tparam Ts @a Valid types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-25)
    ///
    template <typename T, typename... Ts>
    constexpr inline bool contains = std::disjunction_v<std::is_same<T, Ts>...>;

} // namespace util::meta

#endif /* !UTIL_META_CONTAINS_HPP_ */
