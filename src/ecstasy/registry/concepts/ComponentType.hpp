///
/// @file GetComponentType.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022
///
///

#ifndef ECSTASY_REGISTRY_CONCEPTS_GETCOMPONENTTYPE_HPP_
#define ECSTASY_REGISTRY_CONCEPTS_GETCOMPONENTTYPE_HPP_

#include "ecstasy/query/concepts/QueryDataHolder.hpp"

namespace ecstasy
{
    ///
    /// @brief Primary get the component type of the given type. For a component it return the component itself, for a
    /// @ref QueryDataHolder it returns the internal @b QueryData type.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename T>
    struct component_type;

    /// @copydoc component_type
    template <typename T>
    struct component_type {
        using type = T;
    };

    /// @copydoc component_type
    template <query::QueryDataHolder Q>
    struct component_type<Q> {
        using type = typename Q::QueryData;
    };

    ///
    /// @brief Helper for component_type<T>::type.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename T>
    using component_type_t = typename component_type<T>::type;
} // namespace ecstasy

#endif /* !ECSTASY_REGISTRY_CONCEPTS_GETCOMPONENTTYPE_HPP_ */
