///
/// @file is_serializer.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Concept to check if a type is a serializer.
/// @version 1.0.0
/// @date 2024-06-24
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SERIALIZATION_CONCEPTS_ISSERIALIZER_HPP_
#define ECSTASY_SERIALIZATION_CONCEPTS_ISSERIALIZER_HPP_

#include <concepts>
#include <string>
#include <type_traits>

#include "ecstasy/serialization/ISerializer.hpp"

namespace ecstasy::serialization::concepts
{
    ///
    /// @brief Concept to check if a type is a serializer.
    ///
    /// @note This checks for the save and update methods. It don't check for the load method because it would create
    /// circular concept dependencies.
    ///
    /// @tparam S Serializer type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-24)
    ///
    template <typename S>
    concept is_serializer = requires(S &serializer) {
        /// Serializer must inherit from the @ref ecstasy::serialization::ISerializer "ISerializer" .
        requires std::derived_from<S, ISerializer>;

        // clang-format off

        /// Test for some basic types
        // int
        { serializer.save(std::declval<int>())} -> std::same_as<S &>;
        { serializer.update(std::declval<int &>())} -> std::same_as<S &>;
        // double
        { serializer.save(std::declval<double>())} -> std::same_as<S &>;
        { serializer.update(std::declval<double &>())} -> std::same_as<S &>;
        // std::string
        { serializer.save(std::declval<std::string>())} -> std::same_as<S &>;
        { serializer.update(std::declval<std::string &>())} -> std::same_as<S &>;

        // clang-format on
    };

    ///
    /// @brief Alias for @ref ecstasy::serialization::concepts::is_serializer "is_serializer<S>".
    ///
    /// @tparam S Serializer type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-24)
    ///
    template <typename S>
    bool constexpr is_serializer_v = is_serializer<S>;

} // namespace ecstasy::serialization::concepts

#endif /* !ECSTASY_SERIALIZATION_CONCEPTS_ISSERIALIZER_HPP_ */
