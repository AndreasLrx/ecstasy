///
/// @file add_optional.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-27
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_QUERY_CONCEPTS_ADD_OPTIONAL_HPP_
#define ECSTASY_QUERY_CONCEPTS_ADD_OPTIONAL_HPP_

#include <optional>

namespace ecstasy::query
{
    ///
    /// @brief Add optional to a type if required.
    /// @li 'T &' -> std::optional<std::reference_wrapper<T>>
    /// @li 'std::optional<T>' -> no change, returns std::optional<T>
    /// @li Otherwise 'T' -> std::optional<T>
    ///
    /// @tparam T Type to make optional.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-27)
    ///
    template <typename T>
    struct add_optional {
        using type = std::optional<T>;
    };

    /// @copydoc add_optional
    template <typename T>
    struct add_optional<T &> {
        using type = std::optional<std::reference_wrapper<T>>;
    };

    /// @copydoc add_optional
    template <typename T>
    struct add_optional<std::optional<T>> {
        using type = std::optional<T>;
    };

    ///
    /// @brief Helper to add_optional<T>::type.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-27)
    ///
    template <typename T>
    using add_optional_t = add_optional<T>::type;
} // namespace ecstasy::query

#endif /* !ECSTASY_QUERY_CONCEPTS_ADD_OPTIONAL_HPP_ */
