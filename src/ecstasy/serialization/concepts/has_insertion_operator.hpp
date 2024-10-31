///
/// @file has_insertion_operator.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Concept to check if a type can be updated with a serializer using the insertion operator.
/// @version 1.0.0
/// @date 2024-06-24
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SERIALIZATION_HAS_INSERTION_OPERATOR_HPP_
#define ECSTASY_SERIALIZATION_HAS_INSERTION_OPERATOR_HPP_

#include "ecstasy/serialization/ISerializer.hpp"
#include "is_serializer.hpp"

namespace ecstasy::serialization::concepts
{
    ///
    /// @brief Concept to check if a type can be updated with a serializer using the insertion operator.
    ///
    /// @tparam T Type to save.
    /// @tparam S Serializer type.
    ///
    /// @param[in] s Serializer.
    /// @param[in] t Type to save.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-24)
    ///
    template <typename S, typename T>
    concept has_insertion_operator = requires(S &s, T &t) {
        // Cannot use is_serializer here because it would create a circular dependency.
        requires std::derived_from<S, ISerializer>;

        // clang-format off
        { t << s } -> std::same_as<T &>;
        // clang-format on
    };

    // clang-format off
    ///
    /// @brief Alias for @ref ecstasy::serialization::concepts::has_insertion_operator "has_insertion_operator<S, C>::value".
    ///
    /// @tparam S Serializer type.
    /// @tparam C Component type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    // clang-format on
    template <typename S, typename C>
    bool constexpr has_insertion_operator_v = has_insertion_operator<S, C>;

} // namespace ecstasy::serialization::concepts

#endif /* !ECSTASY_SERIALIZATION_HAS_INSERTION_OPERATOR_HPP_ */
