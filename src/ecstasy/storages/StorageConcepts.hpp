/*
** EPITECH PROJECT, 2022
** ecstasy
** File description:
** StorageConcepts
*/

#ifndef ECSTASY_STORAGE_STORAGECONCEPTS_HPP_
#define ECSTASY_STORAGE_STORAGECONCEPTS_HPP_

#include <concepts>
#include "IStorage.hpp"

/// Sets the storage type to use for @b ComponentType, if not called @b MapStorage is used by default.
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
    template <typename C>
    struct getStorageTypeImpl {
        using type = typename GetComponentStorageType<C>::Value;
    };

    template <typename C>
    struct getStorageTypeImpl<const C> {
        using type = const getStorageTypeImpl<C>::type;
    };

    /// Get the storage type of a component, returns MapStorage by default.
    template <typename C>
    using getStorageType = getStorageTypeImpl<C>::type;

    template <typename S>
    concept IsStorage = std::default_initializable<S> && std::derived_from<S, IStorage>
        && requires(S &storage, S const &cstorage) { typename S::Component; };

    template <typename S>
    concept IsContainerStorage = requires() {
        requires IsStorage<S>;
        typename S::Component::value_type;
    };
} // namespace ecstasy
#endif /* !ECSTASY_STORAGE_STORAGECONCEPTS_HPP_ */
