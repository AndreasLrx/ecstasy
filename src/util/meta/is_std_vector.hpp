///
/// @file is_std_vector.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Check if a type is a std::vector.
/// @version 1.0.0
/// @date 2024-04-30
///
/// @copyright Copyright (c) ECSTASY 2024
///
///

#ifndef UTIL_META_IS_STD_VECTOR_HPP_
#define UTIL_META_IS_STD_VECTOR_HPP_

#include <vector>

namespace util::meta
{

    ///
    /// @brief Check if a type is a std::vector.
    ///
    /// @tparam typename Type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-30)
    ///
    template <typename>
    struct is_std_vector : std::false_type {};

    /// @copydoc is_std_vector
    template <typename T, typename A>
    struct is_std_vector<std::vector<T, A>> : std::true_type {};

    ///
    /// @brief Helper for is_std_vector<...>::value.
    ///
    /// @tparam T Type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-30)
    ///
    template <typename T>
    using is_std_vector_v = typename is_std_vector<T>::value;

} // namespace util::meta

#endif /* !UTIL_META_IS_STD_VECTOR_HPP_ */
