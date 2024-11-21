///
/// @file is_size_t_convertible.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Check if a type is a std::size_t or convertible to it.
/// @version 1.0.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef UTIL_META_IS_STD_SIZE_T_CONVERTIBLE_HPP_
#define UTIL_META_IS_STD_SIZE_T_CONVERTIBLE_HPP_

#include <concepts>
#include <type_traits>

namespace util::meta
{

    ///
    /// @brief Check if a type is std::size_t or is convertible to it.
    ///
    /// @tparam T Type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-11-21)
    ///
    template <typename T>
    concept is_size_t_convertible =
        std::same_as<T, std::size_t> || requires(T value) { static_cast<std::size_t>(value); };

} // namespace util::meta

#endif /* !UTIL_META_IS_STD_SIZE_T_CONVERTIBLE_HPP_ */
