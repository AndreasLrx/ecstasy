///
/// @file can_update_type.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
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
    ///
    /// @tparam S Serializer type.
    /// @tparam T Type to save.
    /// @tparam typename S::update(T &) must be a valid expression.
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
    /// @brief Alias for @ref can_update_type::value.
    ///
    /// @tparam S Serializer type.
    /// @tparam C Component type.
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    template <concepts::is_serializer S, typename C>
    bool constexpr can_update_type_v = can_update_type<S, C>::value;

} // namespace ecstasy::serialization::traits

#endif /* !ECSTASY_SERIALIZATION_CAN_UPDATE_TYPE_HPP_ */
