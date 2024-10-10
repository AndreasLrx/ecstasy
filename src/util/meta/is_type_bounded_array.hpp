///
/// @file is_type_bounded_array.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-10-10
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef UTIL_META_IS_TYPE_BOUNDED_ARRAY_HPP_
#define UTIL_META_IS_TYPE_BOUNDED_ARRAY_HPP_

#include <type_traits>

namespace util::meta
{

    ///
    /// @brief Check if a type is a bounded array of a given element type.
    ///
    /// @tparam T Type to check.
    /// @tparam U Expected type of the array elements.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-10)
    ///
    template <typename T, typename U>
    struct is_type_bounded_array : std::false_type {};

    /// @copydoc is_type_bounded_array
    template <typename T, typename U>
        requires std::is_bounded_array_v<T>
    struct is_type_bounded_array<T, U> : std::is_same<std::remove_cvref_t<decltype(std::declval<T>()[0])>, U> {};

    ///
    /// @brief Helper for is_type_bounded_array<...>::value.
    ///
    /// @tparam T Type to check.
    /// @tparam U Expected type of the array elements.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-10-10)
    ///
    template <typename T, typename U>
    inline constexpr bool is_type_bounded_array_v = is_type_bounded_array<T, U>::value;

} // namespace util::meta

#endif
