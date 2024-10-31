///
/// @file component_type.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Get the component type of the given type.
/// @version 1.0.0
/// @date 2022-10-25
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_REGISTRY_CONCEPTS_GETCOMPONENTTYPE_HPP_
#define ECSTASY_REGISTRY_CONCEPTS_GETCOMPONENTTYPE_HPP_

#include "RegistryModifier.hpp"
#include "ecstasy/storages/IStorage.hpp"
#include "ecstasy/storages/StorageConcepts.hpp"

namespace ecstasy
{
    ///
    /// @brief Get the component type of the given type. For a component it return the component itself, for a @ref
    /// ecstasy::RegistryModifier it returns the modifier query data, and for a @ref ecstasy::IsStorage "IsStorage" it
    /// returns the storage data.
    ///
    /// @tparam T Evaluated type.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2022-10-24)
    ///
    template <typename T>
    struct component_type;

    /// @cond Doxygen_Suppress
    /// @copydoc component_type
    template <typename T>
    struct component_type {
        using type = T;
    };
    /// @endcond

    /// @cond Doxygen_Suppress
    /// @copydoc component_type
    template <RegistryModifier Q>
    struct component_type<Q> {
        using type = typename Q::QueryData;
    };
    /// @endcond

    /// @cond Doxygen_Suppress
    /// @copydoc component_type
    template <IsStorage S>
    struct component_type<S> {
        using type = typename S::Component;
    };
    /// @endcond

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
