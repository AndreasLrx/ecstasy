///
/// @file can_update_type.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Concept to check if a type can be updated with a serializer.
/// @version 1.0.0
/// @date 2024-06-24
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SERIALIZATION_CAN_UPDATE_TYPE_HPP_
#define ECSTASY_SERIALIZATION_CAN_UPDATE_TYPE_HPP_

#include <type_traits>

#include "ecstasy/serialization/concepts/is_serializer.hpp"

namespace ecstasy::serialization::traits
{

    ///
    /// @brief Concept to check if a type can be updated with a serializer.
    /// To be true S::update(T &) must be a valid expression.
    ///
    /// @tparam S Serializer type.
    /// @tparam T Type to save.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-24)
    ///
    template <typename S, typename T, typename = std::void_t<>>
    struct can_update_type : std::false_type {};

    /// @copydoc can_update_type
    template <concepts::is_serializer S, typename T>
    struct can_update_type<S, T, std::void_t<decltype(std::declval<S &>().update(std::declval<T &>()))>>
        : std::true_type {};

    ///
    /// @brief Alias for @ref ecstasy::serialization::traits::can_update_type "can_update_type<S, C>::value".
    ///
    /// @tparam S Serializer type.
    /// @tparam C Component type.
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    template <concepts::is_serializer S, typename C>
    bool constexpr can_update_type_v = can_update_type<S, C>::value;

    ///
    /// @brief Concept to check if a serializer has a specific update implementation for a type.
    ///
    /// @tparam S Serializer type.
    /// @tparam T Type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-07-16)
    ///
    template <typename S, typename T, typename = std::void_t<>>
    struct has_update_impl_for_type : std::false_type {};

    /// @copydoc has_update_impl_for_type
    template <typename S, typename T>
    struct has_update_impl_for_type<S, T, std::void_t<decltype(std::declval<S &>().updateImpl(std::declval<T &>()))>>
        : std::true_type {};

    ///
    /// @brief Alias for @ref ecstasy::serialization::traits::has_update_impl_for_type
    /// "has_update_impl_for_typ<S, C>::value".
    ///
    /// @tparam S Serializer type.
    /// @tparam C Component type.
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    template <typename S, typename C>
    bool constexpr has_update_impl_for_type_v = has_update_impl_for_type<S, C>::value;

} // namespace ecstasy::serialization::traits

#endif /* !ECSTASY_SERIALIZATION_CAN_UPDATE_TYPE_HPP_ */
