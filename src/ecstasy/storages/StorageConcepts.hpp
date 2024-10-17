///
/// @file StorageConcepts.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief Storage concepts and utilities.
/// @version 0.1
/// @date 2024-10-17
///
/// @copyright Copyright (c) 2024
///
///

#ifndef ECSTASY_STORAGE_STORAGECONCEPTS_HPP_
#define ECSTASY_STORAGE_STORAGECONCEPTS_HPP_

#include <concepts>
#include "IStorage.hpp"

/// Sets the storage type to use for @b ComponentType, if not called @b MapStorage is used by default.
/// You can also define a @b StorageType member type in the component.
#define SET_COMPONENT_STORAGE(ComponentType, StorageType) \
    template <>                                           \
    struct GetComponentStorageType<ComponentType> {       \
        using Value = StorageType<ComponentType>;         \
    };

/// Forward declaration of map storage to use as default storage.
namespace ecstasy
{
    template <typename C>
    class MapStorage;
}

/// @internal
/// @note Use ecstasy::getStorageType instead.
template <typename C>
struct GetComponentStorageType {
    using Value = ecstasy::MapStorage<C>;
};

namespace ecstasy
{
    /// @internal
    /// @brief Concept to check if a component has an explicit storage type defined.
    ///
    /// @tparam C Component type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-22)
    ///
    template <typename C>
    concept ExplicitComponentStorage = requires() { typename C::StorageType; };

    /// @internal
    /// @brief Get the storage type to use for a component.
    ///
    /// @warning Internal use only.
    ///
    /// @tparam C Component type to get the storage type for.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-22)
    ///
    template <typename C>
    struct getStorageTypeImpl {
        using type = typename GetComponentStorageType<C>::Value;
    };

    /// @copydoc getStorageTypeImpl
    template <ExplicitComponentStorage C>
    struct getStorageTypeImpl<C> {
        using type = typename C::StorageType;
    };

    /// @copydoc getStorageTypeImpl
    template <typename C>
    struct getStorageTypeImpl<const C> {
        using type = const typename getStorageTypeImpl<C>::type;
    };

    ///
    /// @brief Get the storage type to use for a component.
    /// You can set the storage type to use for a component by defining a @b StorageType member type in the component.
    /// If you can't modify the component, you can still use the @ref SET_COMPONENT_STORAGE macro.
    ///
    /// @note By default, it uses @b MapStorage.
    /// @note If C is const, the storage type will be const.
    ///
    /// @tparam C Component type to get the storage type for.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-22)
    ///
    template <typename C>
    using getStorageType = typename getStorageTypeImpl<C>::type;

    ///
    /// @brief Concept to check if a type is a storage.
    ///
    /// @tparam S Storage type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-22)
    ///
    template <typename S>
    concept IsStorage = std::default_initializable<std::remove_const_t<S>>
        && std::derived_from<std::remove_const_t<S>, IStorage> && requires { typename S::Component; };

    ///
    /// @brief Concept to check if a storage is a container storage. (i.e. has a value_type member type like
    /// std::vector)
    ///
    /// @tparam S Storage type to check.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-22)
    ///
    template <typename S>
    concept IsContainerStorage = requires() {
        requires IsStorage<S>;
        typename S::Component::value_type;
    };
} // namespace ecstasy
#endif /* !ECSTASY_STORAGE_STORAGECONCEPTS_HPP_ */
