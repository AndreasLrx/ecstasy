///
/// @file ComponentSerializers.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-19
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_SERIALIZATION_IS_SERIALIZABLE_HPP_
#define ECSTASY_SERIALIZATION_IS_SERIALIZABLE_HPP_

#include <type_traits>

namespace ecstasy::serialization
{
    ///
    /// @brief Check if a component is serializable with the given serializer.
    ///
    /// @tparam S Serializer type.
    /// @tparam C Component type.
    /// @tparam typename Placeholder for SFINAE.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    template <typename S, typename C, typename = void>
    struct is_serializable : std::false_type {};

    /// @copydoc is_serializable
    template <typename S, typename C>
    struct is_serializable<S, C, std::void_t<decltype(std::declval<C &>() >> std::declval<S &>())>> : std::true_type {};

    ///
    /// @brief Alias for @ref is_serializable::value.
    ///
    /// @tparam S Serializer type.
    /// @tparam C Component type.
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-06-11)
    ///
    template <typename S, typename C>
    bool constexpr is_serializable_v = is_serializable<S, C>::value;

} // namespace ecstasy::serialization

#endif /* !ECSTASY_SERIALIZATION_IS_SERIALIZABLE_HPP_ */
